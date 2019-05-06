aclocal
autoheader
autoconf
automake --add-missing
./configure --enable-dependency-tracking CXXFLAGS="-g -O0" CFLAGS="-g -O0"

target=$(uname)
if [ "Linux" == "$target" ]; then
cp -f ./lib/linux/* ./lib/.
else
cp -f ./lib/mac/* ./lib/.
fi
