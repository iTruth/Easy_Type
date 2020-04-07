@echo off
echo 命令格式: parser [金山打字通课程目录(...\Typeeasy\11.4\course)] [输出课程目录(...\course)]
echo 在执行本命令前请先将磁盘切换至输出课程目录所在磁盘, 否则文件将会创建到当前目录下
pause
set fromDir=%1
set toDir=%2
setlocal enabledelayedexpansion
for /r %fromDir% %%i in (*.xml) do (
	set xmlFile=%%i
	getDir %%i >tmp
	set /p xmlDir=<tmp
	set xmlSaveDir=!xmlDir:%fromDir%=%toDir%!
	if not exist !xmlSaveDir! (
		if exist tmp del tmp
		md !xmlSaveDir!
	)
	cd !xmlSaveDir!
	xmlParser !xmlFile!
)
echo Done
