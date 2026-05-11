$f = 'C:\Users\user\Desktop\projetcpp2526-s2-2a12-nextgen-smartleather-commande1\mainwindow.cpp'
$l = Get-Content $f
Write-Host "=== Lines 1584-1592 ==="
$l[1583..1591] | ForEach-Object { Write-Host $_ }
Write-Host "=== Lines 4928-4992 ==="
$l[4927..4991] | ForEach-Object { Write-Host $_ }
