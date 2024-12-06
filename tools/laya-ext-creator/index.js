const fs = require('fs-extra');
const path = require('path');
const { v4: uuidv4 } = require('uuid');

function readTemplate(filePath) {
    return fs.readFileSync(filePath, 'utf8');
}

function copyDirectory(src, dst) {
    fs.copySync(src, dst);
}

function createVsProject(projectName, outputPath) {
    // 创建项目目录
    const projectDir = path.join(outputPath, projectName);
    fs.ensureDirSync(projectDir);

    // 生成GUID
    const projectGuid = uuidv4().toUpperCase();
    const solutionGuid = uuidv4().toUpperCase();

    // 读取模板文件
    const templateDir = path.join(__dirname, 'template');
    
    const slnTemplate = readTemplate(path.join(templateDir, 'temp.sln'));
    const vcxprojTemplate = readTemplate(path.join(templateDir, 'temp', 'temp.vcxproj'));
    const frameworkHTemplate = readTemplate(path.join(templateDir, 'temp', 'framework.h'));
    const dllmainTemplate = readTemplate(path.join(templateDir, 'temp', 'dllmain.cpp'));
    const exportsTemplate = readTemplate(path.join(templateDir, 'temp', 'exports.cpp'));

    // 创建替换字典
    const replacements = {
        PROJECT_NAME: projectName,
        PROJECT_NAME_UPPER: projectName.toUpperCase(),
        PROJECT_GUID: projectGuid,
        SOLUTION_GUID: solutionGuid,
        JSVM_INCLUDE: '$(ProjectDir)jsvm;',
        CONCH_LIB: 'conch.lib;'
    };

    // 使用模板字符串替换模板中的变量
    const slnContent = slnTemplate.replace(/\$\{(\w+)\}/g, (_, p1) => replacements[p1]);
    const vcxprojContent = vcxprojTemplate.replace(/\$\{(\w+)\}/g, (_, p1) => replacements[p1]);
    const frameworkHContent = frameworkHTemplate.replace(/\$\{(\w+)\}/g, (_, p1) => replacements[p1]);
    const dllmainContent = dllmainTemplate.replace(/\$\{(\w+)\}/g, (_, p1) => replacements[p1]);
    const exportsContent = exportsTemplate.replace(/\$\{(\w+)\}/g, (_, p1) => replacements[p1]);

    // 写入解决方案文件
    fs.writeFileSync(path.join(outputPath, `${projectName}.sln`), slnContent);

    // 写入项目文件
    fs.writeFileSync(path.join(projectDir, `${projectName}.vcxproj`), vcxprojContent);

    // 写入头文件
    fs.writeFileSync(path.join(projectDir, "framework.h"), frameworkHContent);

    // 写入DLL主文件
    fs.writeFileSync(path.join(projectDir, "dllmain.cpp"), dllmainContent);

    fs.writeFileSync(path.join(projectDir, "exports.cpp"), exportsContent);

    // 拷贝 layaRuntime 文件夹
    const srcLayaRuntime = path.join(templateDir, 'temp', 'layaRuntime');
    const dstLayaRuntime = path.join(projectDir, 'layaRuntime');
    copyDirectory(srcLayaRuntime, dstLayaRuntime);

    console.log(`解决方案 ${projectName}.sln 已创建在 ${outputPath}`);
    console.log(`项目 ${projectName} 已创建在 ${projectDir}`);
}

module.exports = createVsProject;