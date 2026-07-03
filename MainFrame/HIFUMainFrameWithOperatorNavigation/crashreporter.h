#pragma once

#include <windows.h>

#include <QString>

struct _EXCEPTION_POINTERS;

namespace CrashReporter {

void install(const QString &appVersion = QString());
LONG WINAPI handleException(_EXCEPTION_POINTERS *exceptionPointers);

}  // namespace CrashReporter
