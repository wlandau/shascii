VERSION=$1
INSTALL_PATH=$2
FORCE=$3
REJECTED=0

BIN=shascii
CURR_BIN=shascii_$VERSION

FILES=($INSTALL_PATH/bin/$BIN)
FILES+=($INSTALL_PATH/bin/$CURR_BIN)

NFILES=${#FILES[@]}
for ((i=0; i<$NFILES; i++));
do
  if [[ -f ${FILES[i]} && $FORCE -eq "0" ]];
  then
    echo "OH NO! "${FILES[i]}" already exists."
    REJECTED=1
  fi
done

if [[ $REJECTED -eq 1 ]];
then
  echo "Install aborted. Use make force-install if you're willing to risk overwriting files."
else
  cp $CURR_BIN ${FILES[0]}
  cp $CURR_BIN ${FILES[1]}

  for ((i=0; i<$NFILES; i++));
  do
    if [[ -f ${FILES[i]} ]];
    then
      echo "Existence of binary "${FILES[i]}" verified."
    fi
  done

  for ((i=0; i<$NFILES; i++));
  do
    if [[ ! -f ${FILES[i]} ]];
    then
      echo "Unable to create:  "${FILES[i]}
    fi
  done

  echo "If install succeeded, enter shascii into command line to see usage."
fi