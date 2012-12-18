

FILES=$(find . -iname "*png")

echo "<!DOCTYPE RCC><RCC version=\"1.0\">"
echo " <qresource>"

for f in $FILES;
do
  echo "  <file>"${f}"</file>"
done

echo " </qresource>"
echo "</RCC>"
