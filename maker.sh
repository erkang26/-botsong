aclocal
autoheader
autoconf
automake --add-missing
./configure CXXFLAGS="-O2" CFLAGS="-O2"

target=$(uname)
if [ "Linux" == "$target" ]; then
cp -f ./lib/linux/* ./lib/.
else
cp -f ./lib/mac/* ./lib/.
fi
