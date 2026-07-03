const fs = require('fs');
const dir = 'D:\\9706_draintank_EN\\MainFrame\\HIFUMainFrameWithOperatorNavigation';
const files = ['selftestdlg.cpp','selftestdlg2.cpp','selftestdlg3.cpp',
              'selftestdlg.h','selftestdlg2.h','selftestdlg3.h'];
for (const f of files) {
    const c = fs.readFileSync(dir + '\\' + f, 'utf8');
    const lines = c.split(/\r?\n/);
    let count = 0;
    for (let i = 0; i < lines.length; i++) {
        const l = lines[i];
        const t = l.trim();
        if (t.startsWith('//') || t.startsWith('*') || t.startsWith('/*')) continue;
        if (/[\u4e00-\u9fff]/.test(l) && l.includes('"')) {
            const parts = l.split('"');
            for (let j = 0; j < parts.length; j++) {
                if (j % 2 === 1 && /[\u4e00-\u9fff]/.test(parts[j])) {
                    count++;
                    if (count <= 5) console.log(f+':'+(i+1)+': "'+parts[j].substring(0,80)+'"');
                    break;
                }
            }
        }
    }
    if (count > 0) console.log(f+': total '+count+'\n');
}
