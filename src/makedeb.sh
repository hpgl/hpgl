if ! test $# -eq 1
then 
	echo Usage: \\n $0 \<version\> 
	return 1
else echo $1
fi

rootdir=`pwd`
tempdir=temp/dist
pkgdir=$tempdir/hpgl-bsd-$1


scons geo_bsd
rm -r $tempdir
mkdir -p $pkgdir
mkdir $pkgdir/debian
mkdir $pkgdir/geo_bsd
cp debian/* $pkgdir/debian/
cp geo_bsd/*.py $pkgdir/geo_bsd/
cp geo_bsd/*.so $pkgdir/geo_bsd
cp setup.py $pkgdir/
cp MANIFEST.in $pkgdir/
cd $pkgdir
dpkg-buildpackage -us -uc
cd $rootdir
cp $tempdir/*.deb out/
rm -r $tempdir
