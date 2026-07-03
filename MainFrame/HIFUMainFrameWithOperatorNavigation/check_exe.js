const fs = require('fs');
const exe = fs.readFileSync('D:\\9706_draintank_EN\\MainFrame\\build-HIFUMainframe-Desktop_Qt_5_12_11_MSVC2017_64bit-Release\\release\\HIFUMainFrame.exe');
const content = exe.toString('utf8');

// Search for specific translation strings
const english = ['Patient name', 'Ultrasound', 'Circulation', 'Sonication', 'Treatment'];
const chinese = ['患者姓名', 'B超', '水循环', '辐照'];

console.log('=== NEW exe check ===');
for (const s of english) {
    const idx = content.indexOf(s);
    console.log(idx >= 0 ? 'FOUND:   "' + s + '" at ' + idx : 'NOT FOUND: "' + s + '"');
}
for (const s of chinese) {
    const idx = content.indexOf(s);
    console.log(idx >= 0 ? 'FOUND:   "' + s + '" at ' + idx : 'NOT FOUND: "' + s + '"');
}

// Also check old exe
console.log('\n=== OLD exe check ===');
const oldExe = fs.readFileSync('D:\\9706_draintank_EN\\MainFrame\\HIFUMainFrameWithOperatorNavigation\\release\\HIFUMainFrame.exe');
const oldContent = oldExe.toString('utf8');
for (const s of chinese) {
    const idx = oldContent.indexOf(s);
    console.log(idx >= 0 ? 'FOUND:   "' + s + '" at ' + idx : 'NOT FOUND: "' + s + '"');
}
for (const s of english) {
    const idx = oldContent.indexOf(s);
    console.log(idx >= 0 ? 'FOUND:   "' + s + '" at ' + idx : 'NOT FOUND: "' + s + '"');
}
