@set @e=0 /*
   @echo off
     set @e=
     setlocal
       set "i=0"
       for %%i in (%*) do set /a "i+=1"
       if "%i%" neq "2" goto:eof
       cscript //nologo //e:jscript "%~f0" %1 %2
     endlocal
   exit /b
*/

function Extract(zip, dest) {
   with (new ActiveXObject('Scripting.FileSystemObject')) {
      if (!FolderExists(dest)) CreateFolder(dest);

   with (new ActiveXObject('Shell.Application')) {
      NameSpace(GetFolder(dest).Path).CopyHere(Namespace(GetFile(zip).Path).Items());
   }
  }
}

with (WScript.Arguments) {
  try {
    var zip = Unnamed(0), dest = Unnamed(1);
    Extract(zip, dest);
  }
  catch (e) {
  WScript.echo(e.message);
 }
}
