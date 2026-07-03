const fs = require('fs');
const path = require('path');
const dir = 'D:\\9706_draintank_EN\\MainFrame\\HIFUMainFrameWithOperatorNavigation';

function walk(d) {
    const files = [];
    for (const e of fs.readdirSync(d, { withFileTypes: true })) {
        const p = path.join(d, e.name);
        if (e.isDirectory()) {
            if (!['debug','release','build','.git','.vs','.agents','img','default_dcm','UMImage'].includes(e.name))
                files.push(...walk(p));
        } else if (/\.(cpp|h|qml|ui)$/.test(e.name) && !/^(moc_|qrc_|ui_)/.test(e.name))
            files.push(p);
    }
    return files;
}

const files = walk(dir);
for (const f of files) {
    const buf = fs.readFileSync(f);
    const content = buf.toString('utf8');
    const lines = content.split(/\r?\n/);
    let found = false;
    for (let i = 0; i < lines.length; i++) {
        const l = lines[i];
        const t = l.trim();
        if (t.startsWith('//') || t.startsWith('*') || t.startsWith('/*')) continue;
        if (/[\u4e00-\u9fff]/.test(l) && l.indexOf('"') >= 0) {
            const parts = l.split('"');
            for (let j = 0; j < parts.length; j++) {
                if (j % 2 === 1 && /[\u4e00-\u9fff]/.test(parts[j])) {
                    const s = parts[j];
                    if (s.indexOf('微软雅黑') >= 0) continue;
                    if (s.indexOf('操作栏') >= 0) continue;
                    if (s.indexOf('闭合了') >= 0) continue;
                    if (s.indexOf('踏板开') >= 0 || s.indexOf('踏板关') >= 0) continue;
                    if (s.indexOf('底部icon') >= 0) continue;
                    if (!found) {
                        console.log('\n=== ' + path.basename(f) + ' ===');
                        found = true;
                    }
                    console.log('  L' + (i+1) + ': "' + s.substring(0, 120) + '"');
                    break;
                }
            }
        }
    }
}
console.log('\nDone.');
