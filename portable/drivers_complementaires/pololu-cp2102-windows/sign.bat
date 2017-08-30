del plluvcp.cat
"C:\Program Files (x86)\Windows Kits\8.0\bin\x64\inf2cat" /v /driver:%~dp0 /os:XP_X86,Vista_X86,Vista_X64,7_X86,7_X64,8_X86,8_X64
"C:\Program Files (x86)\Windows Kits\8.0\bin\x64\signtool" sign /v /t http://tsa.starfieldtech.com /n "Pololu Corporation" /r "Go Daddy Class 2 Certification Authority" plluvcp.cat
"C:\Program Files (x86)\Windows Kits\8.0\bin\x64\signtool" sign /v /t http://tsa.starfieldtech.com /n "Pololu Corporation" /r "Go Daddy Class 2 Certification Authority" pololu-cp2102-setup-x86.exe
"C:\Program Files (x86)\Windows Kits\8.0\bin\x64\signtool" sign /v /t http://tsa.starfieldtech.com /n "Pololu Corporation" /r "Go Daddy Class 2 Certification Authority" pololu-cp2102-setup-x64.exe