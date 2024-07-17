const fs = require('fs').promises;
const path = require('path');
const { cwd } = require('process');

async function copyFile(source, destinations) {
    try {
        for (const destination of destinations) {
            const targetPath = path.join(cwd(), destination, path.basename(source));
            console.log(`copy to ${targetPath}`)
            // 检查目标目录是否存在，如果不存在，则创建
            // await fs.mkdir(destination, { recursive: true }).catch(error => {
            //     throw new Error(`创建目录失败：${error.message}`);
            // });

            // 将文件复制到目标目录
            await fs.copyFile(source, targetPath).catch(error => {
                throw new Error(`复制文件${targetPath}出错：${error.message}`);
            });
        }
    } catch (error) {
        console.error(`执行过程中出错: ${error.message}`);
    }
}

// 示例用法
const sourceFile = './Redist/scripts/layadcc.js'; // 源文件路径
const targetDirectories = [   // 目标目录数组
    './template/build/bin/Debug/scripts',
    'template/resource/scripts',
    'publish/ios/resource/scripts',
    'publish/windows/resource/scripts',
    'publish/android_studio/app/src/main/assets/scripts',
    'publish/ohos/entry/src/main/resources/rawfile/scripts/',
    'layaPlayer/proj.ios/resource/scripts',
    'layaPlayer/proj.android/app/src/main/assets/scripts',

];

copyFile(sourceFile, targetDirectories);