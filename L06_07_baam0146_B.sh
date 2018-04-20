# nev: Balazs Adam-Attila
# felhasznalo: baam0146
# feladat: Asszociatív tömb segítségével határozzuk meg, hogy az elmúlt
# hónapban (a bejelentkezéseket nyilvántartó wtmp állomány kezdetétõl
# számítva) a hét melyik napján (hétfõ, kedd,...) hányan voltak
# bejelentkezve, illetve határozzuk meg, hogy a hét melyik napján
# történt a legtöbb bejelentkezés.

if [ $# -gt 0 ]
then
        echo 'No parameters needed'
        exit 1
fi

#-F, --fulltimes
#    Print full login and logout times and dates.

# $4 is the day of the week
last -F | awk '
{
days[$4]++;
}
END { for (var in days) print var, " ", days[var] }

'
