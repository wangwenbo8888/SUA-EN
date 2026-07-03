const fs = require('fs');

const sourceDir = 'D:\\9706_draintank_EN\\MainFrame\\HIFUMainFrameWithOperatorNavigation';
const filesToCheck = [
    'newmainwindow.ui',
    'dupumpcontrol.ui',
    'irradiatecore.cpp',
    'filesavedialog.cpp',
    'commondefine.h'
];

for (const f of filesToCheck) {
    const content = fs.readFileSync(sourceDir + '\\' + f, 'utf8');
    const lines = content.split('\n');

    // Find lines with Chinese that are NOT comments and NOT font names and NOT image paths
    const relevantLines = lines.filter(l => {
        if (!/[\u4e00-\u9fff]/.test(l)) return false;
        if (l.includes('微软雅黑')) return false;
        if (l.includes('url(') || l.includes('.png')) return false;
        return true;
    });

    if (relevantLines.length > 0) {
        console.log(f + ' (' + relevantLines.length + ' non-trivial Chinese lines):');
        relevantLines.slice(0, 5).forEach(l => console.log('  ' + l.trim().substring(0, 150)));
        console.log('');
    }
}
