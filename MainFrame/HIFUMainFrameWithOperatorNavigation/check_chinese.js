const fs = require('fs');
const path = require('path');
const dir = 'D:\\9706_draintank_EN\\MainFrame\\HIFUMainFrameWithOperatorNavigation';

function walkDir(d) {
    const files = [];
    for (const e of fs.readdirSync(d, { withFileTypes: true })) {
        const p = path.join(d, e.name);
        if (e.isDirectory()) {
            if (!['debug','release','build','.git','.vs','.agents','img','default_dcm','UMImage'].includes(e.name))
                files.push(...walkDir(p));
        } else if (/\.(cpp|h|qml)$/.test(e.name) && !/^(moc_|qrc_|ui_)/.test(e.name))
            files.push(p);
    }
    return files;
}

const files = walkDir(dir);
let totalChinese = 0;
const skip = ['微软雅黑','操作栏','闭合了','BOLD'];
for (const f of files) {
    const c = fs.readFileSync(f, 'utf8');
    const lines = c.split('\n');
    for (let i = 0; i < lines.length; i++) {
        const l = lines[i];
        const trimmed = l.trim();
        if (trimmed.startsWith('//') || trimmed.startsWith('*')) continue;
        if (/[\u4e00-\u9fff]/.test(l)) {
            if (l.includes('"')) {
                const strParts = l.split('"');
                for (let j = 0; j < strParts.length; j++) {
                    if (j % 2 === 1 && /[\u4e00-\u9fff]/.test(strParts[j])) {
                        const s = strParts[j];
                        if (skip.some(k => s.includes(k))) continue;
                        totalChinese++;
                        if (totalChinese <= 20)
                            console.log(path.basename(f) + ':' + (i+1) + ' "' + s.substring(0,80) + '"');
                        break;
                    }
                }
            }
        }
    }
}
console.log('\nTotal remaining Chinese in string literals: ' + totalChinese);
