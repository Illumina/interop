# Setup conditional printing for build servers

TRAVIS_BEG='travis_fold:start'
TRAVIS_END='travis_fold:end'
if [[ "$BUILD_SERVER" == "travis" ]]; then
    function run(){
        mesg=$1
        tag=${mesg// /_}
        shift
        echo "$mesg" && echo -en "${TRAVIS_BEG}:${tag}\r"
        $@
        echo -en "${TRAVIS_END}:${tag}\r"
    }
elif [[ "$BUILD_SERVER" == "teamcity" ]]; then
    function run(){
        mesg=$1
        shift
        echo "##teamcity[blockOpened name='$mesg']"
        $@
        echo "##teamcity[blockClosed name='$mesg']"
    }
else
    function run(){
        mesg=$1
        shift
        $@
    }
fi

# Modified from https://stackoverflow.com/questions/27925006/using-host-environment-variables-with-dockerfile

setuser() {
  if [ -z "$1" ]; then
    echo "Usage: $0  <target-path> <modified-path>"
    return
  fi
  if [ -z "$2" ]; then
    echo "Usage: $0 <target-path> <modified-path>"
    return
  fi
  if hash gstat  2> /dev/null; then
    stat="gstat"
  else
    stat="stat"
  fi
  CURRENT_UID=`id -u`
  DEST_UID=`$stat -c "%u" $1`
  if [ $CURRENT_UID = $DEST_UID ]; then
    return
  fi
  DEST_GID=`stat -c "%g" $1`
  if [ -e /home/$DEST_UID ]; then
    return
  fi
  groupadd -g $DEST_GID $DEST_GID
  useradd -u $DEST_UID -g $DEST_GID $DEST_UID
  echo "--User: $DEST_UID"
  echo "--Group: $DEST_GID"
  echo "--Target Path: $2"
  chown -R $DEST_UID:$DEST_GID $2
}

