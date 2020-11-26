@echo Running Code Patterns Txx...
@if exist Txx.txt del Txx.txt
@..\Release\cm T01.exe > Txx.txt
@..\Release\cm T02.exe >> Txx.txt
@..\Release\cm T03.exe >> Txx.txt
@..\Release\cm T04.exe >> Txx.txt
@..\Release\cm T05.exe >> Txx.txt
@..\Release\cm T06.exe >> Txx.txt
@..\Release\cm T07.exe >> Txx.txt
@..\Release\cm T08.exe >> Txx.txt
@..\Release\cm T09.exe >> Txx.txt
@..\Release\cm T10.exe >> Txx.txt
@..\Release\cm T11.exe >> Txx.txt
@..\Release\cm T12.exe >> Txx.txt
type Txx.txt
@echo on
@echo Checking test result...
@..\x64\Release\aunit Txx.txt
pause