@echo off
rem if exist "output.log" del "output.log"
rem set logfile=output.log
rem call :start
rem exit /b 0

:start
echo ######################  DOXYGEN OUTPUT GENERATION  ######################
echo.

set startdir="%~dp0"
cd %startdir%
cd ..\..\
set rootdir=%cd%
set rootimg=%rootdir%\images

set workdir=%rootdir%\.mchp_private\doxygen_src
set doxydir=%workdir%\templates
set doxyimg=%workdir%\images
set mdimg=%workdir%\markdown-files\images

set outputdir=%rootdir%\docs
set searchdir=%outputdir%\search
set outputimg=%outputdir%\images

echo Working Directories: 
echo ====================
echo Source directory:          %workdir%
echo Doxyfile location:         %doxydir%
echo Main images directory:     %rootimg%
echo Content images directory:  %doxyimg%
echo Markdown images directory: %mdimg%
echo Output directory:          %outputdir%
echo Output images directory:   %outputimg%
echo Output search directory:   %searchdir%
echo. 
echo Toolchain Information:
echo ======================
echo.
echo Doxygen Version:
doxygen -v
echo. 
echo Graphviz Version:
rem To generate graphics like call graphs and flow charts, the Graphviz package needs to be installed.
rem You can find this package here: https://www.graphviz.org/download/
rem Please make sure the installation directory of Graphviz is registered as PATH variable
rem Should doxygen still have trouble with calling the Dot tool, execute the following line 
rem to make sure all library packages are properly registered 
dot -V
rem "C:\Program Files\Graphviz 2.44.1\bin\dot.exe" -c
echo. 
echo Output Generation:
echo ==================
echo.
echo -- preparing output generation...
rem Checking if the output directory already exists or needs to be created
echo -- searching output directory 
if exist %outputdir% echo -- output directory present
if exist %outputdir% goto docs_present
echo -- no output directory found
:create_docs
mkdir "%outputdir%"
echo -- created new output directory 'docs'
set /A new_docs=1
goto switch_to_docs
:docs_present
set /A new_docs=0
echo -- found output directory 'docs'
echo. 
:switch_to_docs
cd "%outputdir%"
if "%new_docs%"=="1" goto skip_delete_images
:get_options
echo Clear the output directory before generating new content (recommended)?
echo.
echo     Y = Yes        deletes the content of the output directory and the complete content will be generated
echo     N = No         keeps all content of the output directory and only changed files will be updated
echo     E = Exit       skips the code generation and ends the program
echo.
set /P yesno="Enter Selection:  "
echo.
if "%yesno%"=="e" goto end
if "%yesno%"=="E" goto end
if "%yesno%"=="Y" goto delete_output
if "%yesno%"=="y" goto delete_output
set yesno="n"
echo -- output director contents will not be cleared
goto skip_delete_output
:delete_output
del /F /Q *.*
if exist "%searchdir%" del /F /Q "%searchdir%\*.*"
echo -- output directory cleared
echo.
:skip_delete_output
echo Clear the 'images' directory of the doxygen output directory before generating new content (recommended)?
echo.
echo     Y = Yes        deletes the content of the 'images' sub-directory in the doxygen output directory
echo     N = No         keeps all content of the 'images' sub-directory in the doxygen output directory 
echo                    and only changed files will be updated
echo     E = Exit       skips the code generation and ends the program
echo.
set /P yesno="Enter Selection:  "
echo.
if "%yesno%"=="e" goto end
if "%yesno%"=="E" goto end
if "%yesno%"=="Y" goto delete_images
if "%yesno%"=="y" goto delete_images
set yesno="n"
echo.
echo -- output images directory will not be cleared
goto skip_delete_images
:delete_images
if exist "%outputimg%" del /F /Q "%outputimg%\*.*"
echo -- images directory cleared
:skip_delete_images
echo. 
echo Open output in browser after generation?
echo.
echo     Y = Yes        the generated output will be opened in the default browser
echo     N = No         the program ends after content has been generated
echo     E = Exit       skips the code generation and ends the program
echo.
set /P yesno="Enter Selection:  "
echo.
if "%yesno%"=="e" goto end
if "%yesno%"=="E" goto end
if "%yesno%"=="Y" goto generate_output
if "%yesno%"=="y" goto generate_output
set yesno="n"
echo.
echo -- generated output will not be opened automatically
:generate_output
echo. 
echo ########################  R U N   D O X Y G E N  ########################
echo. 
echo -- searching for doxygen path...
echo -- %doxydir%
echo -- setting doxygen path...
cd "%doxydir%"
set testdir=%cd%
echo -- calling doxygen path...
echo -- %testdir%
echo -- calling doxygen generator...
echo.
doxygen doxyfile
echo. 
echo ##############  D O X Y G E N   E X E C U T I O N   E N D  ##############
echo. 
echo -- merging template image folder content into output image directory
Xcopy /e /i /c /r /y "%rootimg%" "%outputimg%"
if exist "%mdimg%" Xcopy /e /i /c /r /y "%mdimg%" "%outputimg%"
if exist "%doxyimg%" Xcopy /e /i /c /r /y "%doxyimg%" "%outputimg%"
echo. 
echo -- content generation complete
echo. 
echo ###############################  D O N E  ###############################
echo. 
if "%yesno%"=="Y" goto open_output
if "%yesno%"=="y" goto open_output
echo -- 'Open Output After Generation' option skipped by user (%yesno%)
goto end
:open_output
echo -- open output (%yesno%)
%outputdir%\index.html
:end
@timeout 10
