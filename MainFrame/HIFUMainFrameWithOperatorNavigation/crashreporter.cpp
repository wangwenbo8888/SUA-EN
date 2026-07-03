#include "crashreporter.h"

#include <DbgHelp.h>
#include <eh.h>
#include <exception>
#include <signal.h>

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QTextStream>
#include <QtGlobal>

namespace {

const DWORD kCppTerminateException = 0xE0000001;
const DWORD kPureCallException = 0xE0000002;
const DWORD kInvalidParameterException = 0xE0000003;
const DWORD kSignalAbortException = 0xE0000004;
const DWORD kSignalSegvException = 0xE0000005;

QString g_appVersion;
volatile LONG g_isHandlingCrash = 0;

QString formatPointer(quintptr value)
{
    return QString("0x%1").arg(static_cast<qulonglong>(value),
                               QT_POINTER_SIZE * 2,
                               16,
                               QLatin1Char('0')).toUpper();
}

QString exceptionCodeToString(DWORD code)
{
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION:
        return QStringLiteral("ACCESS_VIOLATION");
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        return QStringLiteral("ARRAY_BOUNDS_EXCEEDED");
    case EXCEPTION_BREAKPOINT:
        return QStringLiteral("BREAKPOINT");
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return QStringLiteral("DATATYPE_MISALIGNMENT");
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        return QStringLiteral("FLT_DENORMAL_OPERAND");
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return QStringLiteral("FLT_DIVIDE_BY_ZERO");
    case EXCEPTION_FLT_INEXACT_RESULT:
        return QStringLiteral("FLT_INEXACT_RESULT");
    case EXCEPTION_FLT_INVALID_OPERATION:
        return QStringLiteral("FLT_INVALID_OPERATION");
    case EXCEPTION_FLT_OVERFLOW:
        return QStringLiteral("FLT_OVERFLOW");
    case EXCEPTION_FLT_STACK_CHECK:
        return QStringLiteral("FLT_STACK_CHECK");
    case EXCEPTION_FLT_UNDERFLOW:
        return QStringLiteral("FLT_UNDERFLOW");
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return QStringLiteral("ILLEGAL_INSTRUCTION");
    case EXCEPTION_IN_PAGE_ERROR:
        return QStringLiteral("IN_PAGE_ERROR");
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return QStringLiteral("INT_DIVIDE_BY_ZERO");
    case EXCEPTION_INT_OVERFLOW:
        return QStringLiteral("INT_OVERFLOW");
    case EXCEPTION_INVALID_DISPOSITION:
        return QStringLiteral("INVALID_DISPOSITION");
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        return QStringLiteral("NONCONTINUABLE_EXCEPTION");
    case EXCEPTION_PRIV_INSTRUCTION:
        return QStringLiteral("PRIV_INSTRUCTION");
    case EXCEPTION_SINGLE_STEP:
        return QStringLiteral("SINGLE_STEP");
    case EXCEPTION_STACK_OVERFLOW:
        return QStringLiteral("STACK_OVERFLOW");
    case kCppTerminateException:
        return QStringLiteral("CPP_TERMINATE");
    case kPureCallException:
        return QStringLiteral("PURE_VIRTUAL_CALL");
    case kInvalidParameterException:
        return QStringLiteral("INVALID_PARAMETER");
    case kSignalAbortException:
        return QStringLiteral("SIGNAL_ABORT");
    case kSignalSegvException:
        return QStringLiteral("SIGNAL_SEGMENTATION_FAULT");
    default:
        break;
    }

    return QStringLiteral("UNKNOWN_EXCEPTION");
}

QString accessViolationDetails(const EXCEPTION_RECORD *record)
{
    if (!record || record->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || record->NumberParameters < 2) {
        return QString();
    }

    QString operation = QStringLiteral("unknown");
    switch (record->ExceptionInformation[0]) {
    case 0:
        operation = QStringLiteral("read");
        break;
    case 1:
        operation = QStringLiteral("write");
        break;
    case 8:
        operation = QStringLiteral("execute");
        break;
    default:
        break;
    }

    return QStringLiteral("%1 at %2").arg(
        operation,
        formatPointer(static_cast<quintptr>(record->ExceptionInformation[1])));
}

QString moduleNameFromBase(DWORD64 moduleBase)
{
    if (moduleBase == 0) {
        return QStringLiteral("<unknown module>");
    }

    wchar_t modulePath[MAX_PATH] = {0};
    const DWORD length = GetModuleFileNameW(reinterpret_cast<HMODULE>(moduleBase), modulePath, MAX_PATH);
    if (length == 0) {
        return QStringLiteral("<unknown module>");
    }

    return QFileInfo(QString::fromWCharArray(modulePath, static_cast<int>(length))).fileName();
}

QString readSourceContext(const QString &filePath, DWORD lineNumber, int radius)
{
    if (filePath.isEmpty() || lineNumber == 0) {
        return QString();
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    const int startLine = qMax(1, static_cast<int>(lineNumber) - radius);
    const int endLine = static_cast<int>(lineNumber) + radius;

    QTextStream stream(&file);
    QString snippet;
    int currentLine = 0;
    while (!stream.atEnd()) {
        const QString line = stream.readLine();
        ++currentLine;
        if (currentLine < startLine) {
            continue;
        }
        if (currentLine > endLine) {
            break;
        }

        snippet += QStringLiteral("%1 %2 | %3\n")
                       .arg(currentLine == static_cast<int>(lineNumber) ? QStringLiteral(">") : QStringLiteral(" "),
                            QString::number(currentLine).rightJustified(5, QLatin1Char(' ')),
                            line);
    }

    return snippet.trimmed();
}

bool writeMiniDump(const QString &dumpPath, EXCEPTION_POINTERS *exceptionPointers)
{
    HANDLE dumpFile = CreateFileW(reinterpret_cast<LPCWSTR>(dumpPath.utf16()),
                                  GENERIC_WRITE,
                                  0,
                                  NULL,
                                  CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);
    if (dumpFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ExceptionPointers = exceptionPointers;
    dumpInfo.ClientPointers = TRUE;

    const BOOL result = MiniDumpWriteDump(GetCurrentProcess(),
                                          GetCurrentProcessId(),
                                          dumpFile,
                                          static_cast<MINIDUMP_TYPE>(MiniDumpWithDataSegs |
                                                                     MiniDumpWithHandleData |
                                                                     MiniDumpWithThreadInfo),
                                          exceptionPointers ? &dumpInfo : NULL,
                                          NULL,
                                          NULL);
    CloseHandle(dumpFile);
    return result == TRUE;
}

void initializeFrameFromContext(STACKFRAME64 &frame, CONTEXT &context, DWORD &machineType)
{
    ZeroMemory(&frame, sizeof(frame));

#if defined(_M_X64)
    machineType = IMAGE_FILE_MACHINE_AMD64;
    frame.AddrPC.Offset = context.Rip;
    frame.AddrFrame.Offset = context.Rbp;
    frame.AddrStack.Offset = context.Rsp;
#elif defined(_M_IX86)
    machineType = IMAGE_FILE_MACHINE_I386;
    frame.AddrPC.Offset = context.Eip;
    frame.AddrFrame.Offset = context.Ebp;
    frame.AddrStack.Offset = context.Esp;
#else
#error Unsupported target architecture
#endif

    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;
}

QString resolveFrame(HANDLE process, DWORD64 address, int frameIndex)
{
    if (address == 0) {
        return QString();
    }

    char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)] = {0};
    SYMBOL_INFO *symbol = reinterpret_cast<SYMBOL_INFO *>(symbolBuffer);
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;

    DWORD64 displacement = 0;
    IMAGEHLP_LINE64 lineInfo;
    ZeroMemory(&lineInfo, sizeof(lineInfo));
    lineInfo.SizeOfStruct = sizeof(lineInfo);

    DWORD lineDisplacement = 0;
    const bool hasSymbol = SymFromAddr(process, address, &displacement, symbol) == TRUE;
    const bool hasLine = SymGetLineFromAddr64(process, address, &lineDisplacement, &lineInfo) == TRUE;

    QStringList parts;
    parts << QStringLiteral("#%1").arg(frameIndex, 2, 10, QLatin1Char('0'));
    parts << moduleNameFromBase(SymGetModuleBase64(process, address));
    parts << formatPointer(static_cast<quintptr>(address));
    parts << (hasSymbol ? QString::fromLatin1(symbol->Name) : QStringLiteral("<unknown symbol>"));

    QString frameText = parts.join(QStringLiteral(" | "));
    if (hasLine) {
        const QString fileName = QString::fromLocal8Bit(lineInfo.FileName);
        frameText += QStringLiteral("\n    at %1:%2")
                         .arg(QDir::toNativeSeparators(fileName))
                         .arg(lineInfo.LineNumber);

        const QString snippet = readSourceContext(fileName, lineInfo.LineNumber, 2);
        if (!snippet.isEmpty()) {
            frameText += QStringLiteral("\n") + snippet;
        }
    }

    return frameText;
}

QString buildStackTrace(EXCEPTION_POINTERS *exceptionPointers)
{
    if (!exceptionPointers || !exceptionPointers->ContextRecord) {
        return QStringLiteral("No exception context available.\n");
    }

    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_FAIL_CRITICAL_ERRORS);
    if (!SymInitialize(process, NULL, TRUE)) {
        return QStringLiteral("SymInitialize failed.\n");
    }

    CONTEXT context = *exceptionPointers->ContextRecord;
    STACKFRAME64 frame;
    DWORD machineType = 0;
    initializeFrameFromContext(frame, context, machineType);

    QString stackTrace;
    for (int index = 0; index < 40; ++index) {
        const DWORD64 address = frame.AddrPC.Offset;
        if (address == 0) {
            break;
        }

        stackTrace += resolveFrame(process, address, index);
        stackTrace += QLatin1Char('\n');

        const BOOL walked = StackWalk64(machineType,
                                        process,
                                        thread,
                                        &frame,
                                        &context,
                                        NULL,
                                        SymFunctionTableAccess64,
                                        SymGetModuleBase64,
                                        NULL);
        if (!walked) {
            break;
        }
    }

    SymCleanup(process);
    return stackTrace.trimmed() + QLatin1Char('\n');
}

QString buildRegisterText(const CONTEXT *context)
{
    if (!context) {
        return QStringLiteral("No register context available.\n");
    }

#if defined(_M_X64)
    return QStringLiteral(
               "RIP=%1\n"
               "RSP=%2\n"
               "RBP=%3\n"
               "RAX=%4\n"
               "RBX=%5\n"
               "RCX=%6\n"
               "RDX=%7\n"
               "RSI=%8\n"
               "RDI=%9\n")
        .arg(formatPointer(static_cast<quintptr>(context->Rip)))
        .arg(formatPointer(static_cast<quintptr>(context->Rsp)))
        .arg(formatPointer(static_cast<quintptr>(context->Rbp)))
        .arg(formatPointer(static_cast<quintptr>(context->Rax)))
        .arg(formatPointer(static_cast<quintptr>(context->Rbx)))
        .arg(formatPointer(static_cast<quintptr>(context->Rcx)))
        .arg(formatPointer(static_cast<quintptr>(context->Rdx)))
        .arg(formatPointer(static_cast<quintptr>(context->Rsi)))
        .arg(formatPointer(static_cast<quintptr>(context->Rdi)));
#else
    return QStringLiteral(
               "EIP=%1\n"
               "ESP=%2\n"
               "EBP=%3\n"
               "EAX=%4\n"
               "EBX=%5\n"
               "ECX=%6\n"
               "EDX=%7\n"
               "ESI=%8\n"
               "EDI=%9\n")
        .arg(formatPointer(static_cast<quintptr>(context->Eip)))
        .arg(formatPointer(static_cast<quintptr>(context->Esp)))
        .arg(formatPointer(static_cast<quintptr>(context->Ebp)))
        .arg(formatPointer(static_cast<quintptr>(context->Eax)))
        .arg(formatPointer(static_cast<quintptr>(context->Ebx)))
        .arg(formatPointer(static_cast<quintptr>(context->Ecx)))
        .arg(formatPointer(static_cast<quintptr>(context->Edx)))
        .arg(formatPointer(static_cast<quintptr>(context->Esi)))
        .arg(formatPointer(static_cast<quintptr>(context->Edi)));
#endif
}

void writeReport(const QString &reportPath, const QString &dumpPath, EXCEPTION_POINTERS *exceptionPointers)
{
    QFile reportFile(reportPath);
    if (!reportFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&reportFile);
    out.setCodec("UTF-8");

    const QDateTime now = QDateTime::currentDateTime();
    const EXCEPTION_RECORD *record = exceptionPointers ? exceptionPointers->ExceptionRecord : NULL;
    const CONTEXT *context = exceptionPointers ? exceptionPointers->ContextRecord : NULL;
    const DWORD code = record ? record->ExceptionCode : 0;

    out << "Time: " << now.toString("yyyy-MM-dd hh:mm:ss.zzz") << "\n";
    out << "Application: " << QCoreApplication::applicationName() << "\n";
    out << "Version: " << (g_appVersion.isEmpty() ? QCoreApplication::applicationVersion() : g_appVersion) << "\n";
    out << "ProcessId: " << GetCurrentProcessId() << "\n";
    out << "ThreadId: " << GetCurrentThreadId() << "\n";
    out << "Architecture: " << (sizeof(void *) == 8 ? "x64" : "x86") << "\n";
    out << "DumpFile: " << QDir::toNativeSeparators(dumpPath) << "\n";
    out << "\n";

    out << "ExceptionCode: " << QString("0x%1").arg(code, 8, 16, QLatin1Char('0')).toUpper()
        << " (" << exceptionCodeToString(code) << ")\n";
    if (record) {
        out << "ExceptionAddress: " << formatPointer(reinterpret_cast<quintptr>(record->ExceptionAddress)) << "\n";
        out << "ExceptionFlags: " << QString("0x%1").arg(record->ExceptionFlags, 8, 16, QLatin1Char('0')).toUpper() << "\n";
        out << "ExceptionParameters: " << record->NumberParameters << "\n";

        const QString avText = accessViolationDetails(record);
        if (!avText.isEmpty()) {
            out << "AccessViolation: " << avText << "\n";
        }
    }

    out << "\nRegisters:\n";
    out << buildRegisterText(context);
    out << "\nStackTrace:\n";
    out << buildStackTrace(exceptionPointers);
    out.flush();
}

void raiseSyntheticException(DWORD code)
{
    RaiseException(code, EXCEPTION_NONCONTINUABLE, 0, NULL);
}

void terminateHandler()
{
    raiseSyntheticException(kCppTerminateException);
}

void pureCallHandler()
{
    raiseSyntheticException(kPureCallException);
}

void invalidParameterHandler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t)
{
    raiseSyntheticException(kInvalidParameterException);
}

void signalHandler(int signalNumber)
{
    switch (signalNumber) {
    case SIGABRT:
        raiseSyntheticException(kSignalAbortException);
        break;
    case SIGSEGV:
        raiseSyntheticException(kSignalSegvException);
        break;
    default:
        raiseSyntheticException(kSignalAbortException);
        break;
    }
}

}  // namespace

namespace CrashReporter {

LONG WINAPI handleException(EXCEPTION_POINTERS *exceptionPointers)
{
    if (InterlockedCompareExchange(&g_isHandlingCrash, 1, 0) != 0) {
        return EXCEPTION_EXECUTE_HANDLER;
    }

    const QString crashDir = QCoreApplication::applicationDirPath() + "/crash";
    QDir().mkpath(crashDir);

    const QString baseName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_zzz");
    const QString dumpPath = crashDir + "/" + baseName + ".dmp";
    const QString reportPath = crashDir + "/" + baseName + ".rpt";

    writeMiniDump(dumpPath, exceptionPointers);
    writeReport(reportPath, dumpPath, exceptionPointers);

    const QString title = QCoreApplication::applicationName() + QStringLiteral(" Crash Report");
    const QString message = QStringLiteral("Application crashed. Crash report generated.\n\nReport:\n%1\n\nDump:\n%2")
                                .arg(QDir::toNativeSeparators(reportPath),
                                     QDir::toNativeSeparators(dumpPath));

//    MessageBoxW(NULL,
//                reinterpret_cast<LPCWSTR>(message.utf16()),
//                reinterpret_cast<LPCWSTR>(title.utf16()),
//                MB_OK | MB_ICONERROR);

    return EXCEPTION_EXECUTE_HANDLER;
}

void install(const QString &appVersion)
{
    if (!appVersion.isEmpty()) {
        g_appVersion = appVersion;
    }

    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    SetUnhandledExceptionFilter(handleException);

    std::set_terminate(terminateHandler);
    _set_purecall_handler(pureCallHandler);
    _set_invalid_parameter_handler(invalidParameterHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGSEGV, signalHandler);
}

}  // namespace CrashReporter
