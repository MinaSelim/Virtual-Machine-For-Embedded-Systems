@echo Running Code Patterns Txx...
@if exist STxx.txt del STxx.txt
@..\Release\cm ST01.exe >> STxx.txt
@..\Release\cm ST02.exe >> STxx.txt
@..\Release\cm ST03.exe >> STxx.txt
@..\Release\cm ST04.exe >> STxx.txt
type STxx.txt
pause