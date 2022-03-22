# MSH-TESTER-2021.09

RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"

BOLDBLACK="\033[1m\033[30m"
BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDBLUE="\033[1m\033[34m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"

# Compile and set executable rights
make -C ../ > /dev/null
cp ../minishell .
chmod 755 minishell


GOOD=0
TOTAL=0

pipe=/tmp/testpipe
trap "rm -f $pipe" EXIT
if [[ ! -p $pipe ]]; then
    mkfifo $pipe
fi

function exec_test()
{
  
  rm -f msh_log
  # execute commands, separated by ';' in minishell, using nfifo
  ./minishell <$pipe >msh_log 2>&- &
  mshpid=$!
  IFS=';' read -ra CMND <<< "$@"
  for command in "${CMND[@]}"; do
    echo $command > $pipe
  done

  # only leaks mode
  if [ $LEAKSMODE  ]; then
    echo "leaks $mshpid | grep 'leaks for' | grep -v ' 0 leaks' >> leaks" > $pipe
    echo 'exit' > $pipe 
    sleep 0.02
    wait $!
    sleep 0.02
    if [[ $(cat leaks 2>&-) != "" ]]; then
      printf "\n$BOLDRED LEAKS! $YELLOW%s$RESET\n" "$@"
      rm -f leaks
    else
      printf "$BOLDGREEN%s$RESET" "✓ "
    fi
    return
  fi

  echo 'exit' > $pipe 
  sleep 0.02
  wait $!
  ES_1=$?
  TEST1=$(cat msh_log)

  rm -f msh_log
  # execute commands in bash
  bash -posix < $pipe >msh_log 2>&-  &
  IFS=';' read -ra CMND <<< "$@"
  for command in "${CMND[@]}"; do
    echo $command > $pipe
  done
  echo 'exit' > $pipe
  wait $!
  ES_2=$?
  TEST2=$(cat msh_log)

  # compare result
  if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
    printf "$BOLDGREEN%s$RESET" "✓ "
    ((GOOD++))
  else
    printf "\n$BOLDRED%s$RESET" "✗ "
    printf "$CYAN \"$@\" $RESET"
  fi
  if [ "$TEST1" != "$TEST2" ]; then
    echo
    printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
    printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
    echo
  fi
  if [ "$ES_1" != "$ES_2" ]; then
    echo
    printf $BOLDRED"Your exit status : $BOLDRED$ES_1$RESET\n"
    printf $BOLDGREEN"Expected exit status : $BOLDGREEN$ES_2$RESET\n"
    echo
  fi
  ((TOTAL++))
  sleep 0.02
}

if [ "$1" == "" ] || [ "$1" == "help" ]; then
  printf "$BOLDMAGENTA\n\tAvailable arg: \n$YELLOW\tall$RESET echo cd pipe env export redirect multi syntax exit\n\
      $GREEN\tbonus$RESET quote wildcard oper\n"
fi
if [ "$1" == "all" ]; then
  printf "$BOLDMAGENTA    _____ _ _ _____ _____ _ _ ______ _ _ \n"
  printf "| \/ |_ _| \ | |_ _|/ ____| | | | ____| | | | \n"
  printf "| \ / | | | | \| | | | | (___ | || | |  | | | | \n"
  printf "| |\/| | | | | . \` | | | \___ \|   | | | | | | \n"
  printf "| | | |_| |_| |\ |_| |_ ____) | | | | |____| |____| |____ \n"
  printf "|_| |_|_____|_| \_|_____|_____/|_| |_|______|______|______|\n$RESET"
fi

if [ "$2" == "leaks" ]; then
  printf $BOLDBLUE"\n\tONLY LEAKS MODE\n"$RESET
  LEAKSMODE=1
  rm -f leaks
fi

# PARSING
if [ "$1" == "parsing" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tPARSING\n"$RESET
  exec_test 'echo "\s"'
  exec_test 'echo "bip | bip && coyotte > < "'
  exec_test 'echo $USER$var$USER$USER$USERtest$USER user42$USERuser42$USERtestuser42'
  exec_test 'echo $USER$var$USER$USER$USERtest$USER'
  exec_test '$'
  exec_test '$LESS$VAR'
  exec_test '..'
  exec_test 'echo '"abc"''
  exec_test 'echo "'abc'"'
  exec_test 'echo "" bonjour'
  exec_test 'echo	a'
  exec_test 'export ""  et unset ""'
  exec_test 'export "test=ici"=coucou'
  exec_test 'echo $test'
  exec_test 'export var="cat Makefile | grep >"'
  exec_test 'echo $var'
fi

# ECHO TESTS
if [ "$1" == "echo" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tECHO TESTS\n"$RESET
  exec_test 'pwd ; pwd'
  exec_test 'echo test tout'
  exec_test 'echo -n test tout'
  exec_test 'echo -n -n -n test tout'
  exec_test 'echo "'$USER'"'
  exec_test 'echo "''"'
  exec_test 'echo "'$USER'" "" '' $USER $USERr '"$USER"''
  exec_test 'echo $=$=$= $=  $='
  exec_test 'echo $+$+$+  $'
  exec_test 'echo -n'
  exec_test 'echo -nn'
  exec_test 'echo -'
  exec_test 'echo --n'
  exec_test 'echo "-n"'
  exec_test 'echo -n blabla'
  exec_test 'echo -nn blabla'
  exec_test 'echo - blabla'
  exec_test 'echo --n blabla'
  exec_test 'echo "-n" blabla'
  exec_test 'echo -nblabla'
  exec_test 'echo""'
  exec_test 'echo -n""'
  exec_test 'echo hello'
  exec_test 'echo exit'
  exec_test 'echo -n'
  exec_test 'echo'
  exec_test 'echo hello blabla'
  exec_test 'echo -n -n -n blabla'
  exec_test 'echo blabla -n'
  exec_test 'echo echo hello'
  exec_test 'echo "bla"bla ""''"'''" bla"bla   "'   "  ""  'sdc" ssdsd " sd' s''d s'sd 's'sd ' s' "vd v'd 'v df 'd df' v' dfv' dv' df 'dvf ' df vdf    v'df v' df'v "'
  exec_test 'echo echo'
  exec_test 'eCho '
  exec_test 'eChO'
  exec_test 'eCHO'
  exec_test 'ECHO'
  exec_test 'echo rhobebou'
  exec_test 'echo stop barking'
  exec_test 'echo "bonjour"'
  exec_test 'echo bonjour'
  exec_test 'echo 'bonjour''
  exec_test 'echo -n bonjour'
  exec_test 'echo -nn bonjour'
  exec_test 'echo -n -n -n bonjour'
  exec_test 'echo -n -n -n bonjour'
  exec_test 'echo "-n" bonjour'
  exec_test 'echo -n"-n" bonjour'
  exec_test 'echo "-nnnn" bonjour'
  exec_test 'echo "-n -n -n"-n bonjour'
  exec_test 'echo "-n '-n'" bonjour'
  exec_test 'echo "~"'
  exec_test 'echo '~''
  exec_test 'echo $USER'
  exec_test 'echo "$USER"'
  exec_test 'echo "'$USER'"'
  exec_test 'echo " '$USER' "'
  exec_test 'echo text"$USER"'
  exec_test 'echo text"'$USER'" ' $USER ''
  exec_test 'echo "text"   "$USER"    "$USER"'
  exec_test 'echo '              $USER          ''
  exec_test 'echo               text "$USER"            "$USER"text'
  exec_test 'echo ''''''''''$USER'''''''''''
  exec_test 'echo """"""""$USER""""""""'
  exec_test 'echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER '''
  exec_test 'echo $USER '' $USER $USER '' $USER '' $USER -n $USER'
  exec_test 'echo "$USER""$USER""$USER"'
  exec_test 'echo text"$USER"test'
  exec_test 'echo '$USER' "$USER" "text text"'
  exec_test 'echo '$USER''
  exec_test 'echo $USER " "'
  exec_test 'echo "$USER""Users/$USER/file""'$USER'"'$USER''
  exec_test 'echo "$USER$USER$USER"'
  exec_test 'echo '$USER'"$USER"'$USER''
  exec_test 'echo '"$USER"''$USER'"""$USER"'
  exec_test 'echo " $USER  "'$PWD''
  exec_test 'echo " $USER  $ "'$PWD''
  exec_test 'echo $USER=4'
  exec_test 'echo $USER=thallard'
  exec_test 'echo $USER'
  exec_test 'echo $USER213'
  exec_test 'echo $USER$12USER$USER=4$USER12'
  exec_test 'echo $USER $123456789USER $USER123456789'
  exec_test 'echo $USER $9999USER $8888USER $7777USER'
  exec_test 'echo $USER $USER9999 $USER8888 $USER7777'
  exec_test 'echo $USER $USER9999 $USER8888 $USER7777 "$USER"'
  exec_test 'echo "$USER=12$USER"'
  exec_test 'echo "$9USER" "'$USER=12$SOMETHING'"'
  exec_test 'echo $PWD/file'
  exec_test 'echo "$PWD/file"'
  exec_test 'echo "text" "text$USER" ... "$USER"'
  exec_test 'echo $PWD'
  exec_test 'echo -n -n -nnnn -nnnnm 	-nnnnm'
  exec_test 'echo -n -nnn hello -n 	hello -n'
  exec_test 'echo poil | cat -e	poil$'
fi

# CD TESTS
if [ "$1" == "cd" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tCD TESTS\n"$RESET
  exec_test 'cd .. ; pwd'
  exec_test 'cd /Users ; pwd'
  exec_test 'cd ; pwd'
  exec_test 'cd . ; pwd'
  exec_test 'mkdir test_dir ; cd test_dir ; rm -rf ../test_dir ; cd . ; cd .. ; pwd'
  exec_test 'cd .a ; pwd'
  exec_test 'cd ; pwd'
  exec_test 'cd .. ; pwd'
  exec_test 'cd . ; pwd'
  exec_test 'cd /Users ; pwd'
  exec_test 'cd // ; pwd'
  exec_test 'cd ////// ; pwd'
  exec_test 'cd ./././ ; pwd'
  exec_test 'cd / ; pwd'
  exec_test 'cd '/etc' ; pwd'
  exec_test 'cd '/var' ; pwd'
  exec_test 'cd "$OLDPWD" ; pwd'
  exec_test 'cd "doesntexist" ; pwd'
  exec_test 'cd "doesntexist">/dev/null ; pwd'
  exec_test 'cd ../../.. ; pwd'
  exec_test 'cd "wtf" >/dev/null ; pwd'
  exec_test 'cd ../../../../../../../../../../../../../../.. ; pwd'
  exec_test 'cd .. ; pwd'
  exec_test 'cd ? ; pwd'
  exec_test 'cd + ; pwd'
  exec_test 'cd _ ; pwd'
  exec_test 'cd woof ; pwd'
  exec_test 'cd bark bark ; pwd'
  exec_test 'cd '/' ; pwd'
  exec_test 'cd $OLDPWD ; pwd'
  exec_test 'cd $OLDPWD/.. ; pwd'
  exec_test 'cd ../../../../../.. ; pwd' 
  exec_test 'cd /home/user42/ ; pwd'
  exec_test 'cd too many arguments ; pwd'
  exec_test 'cd ./ ; pwd'
  exec_test 'cd a/b ; pwd'
  exec_test 'cd .. ; pwd'
  exec_test 'cd home/.. ; pwd'
fi

# PIPE TESTS
if [ "$1" == "pipe" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tPIPE TESTS\n"$RESET
  exec_test '/bin/ls'
	exec_test 'cat tests/lorem.txt | grep arcu | cat -e'
	exec_test 'echo test | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e'
#	exec_test 'cat /dev/random | head -c 100 | wc -c'
	exec_test 'ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls'
	exec_test 'ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls'
  head -c 2000000 /dev/zero > big_file
  exec_test ' cat big_file | ls'
  exec_test ' cat big_file | echo lol'
  exec_test 'echo test | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat'
  exec_test 'cat /dev/urandom | head -c 100 | wc -c'
  exec_test 'ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls'
  exec_test 'cat Makefile | grep pr | head -n 5 | cd file_not_exit'
  exec_test 'cat Makefile | grep pr | head -n 5 | hello'
  exec_test 'ls | exit'
  exec_test 'sleep 0.1 | exit'
fi

# ENV EXPANSIONS
if [ "$1" == "env" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tENV EXPANSIONS TESTS\n"$RESET
	exec_test 'echo test test'
	exec_test 'echo test'
	exec_test 'echo $TEST'
	exec_test 'echo "$TEST"'
	exec_test 'echo '$TEST''
	exec_test 'echo "$TEST$TEST$TEST"'
	exec_test 'echo "$TEST$TEST=lol$TEST"'
	exec_test 'echo " $TEST lol $TEST"'
	exec_test 'echo $TEST$TEST$TEST'
	exec_test 'echo $TEST$TEST=lol$TEST""lol'
	exec_test 'echo $TEST lol $TEST'
	exec_test 'echo test "$TEST" test "$TEST " test'
	exec_test 'echo "$=TEST"'
	exec_test 'echo "$"'
	exec_test 'echo "$?TEST"'
	exec_test 'echo $TEST $TEST'
	exec_test 'echo "$1TEST"'
	exec_test 'echo "$T1TEST"'
  exec_test 'export TEST=BLA ; echo "test"'
  exec_test 'export TEST=BLA ; echo $TEST'
  exec_test 'export TEST=BLA ; echo "$TEST"'
  exec_test 'export TEST=BLA ; echo '$TEST''
  exec_test 'export TEST=BLA ; echo "$TEST$TEST$TEST"'
  exec_test 'export TEST=BLA ; echo "$TEST$TEST=lol$TEST"'
  exec_test 'export TEST=BLA ; echo "   $TEST lol $TEST"'
  exec_test 'export TEST=BLA ; echo $TEST$TEST$TEST'
  exec_test 'export TEST=BLA ; echo $TEST$TEST=lol$TEST""lol'
  exec_test 'export TEST=BLA ; echo    $TEST lol $TEST'
  exec_test 'export TEST=BLA ; echo test "" test "" test'
  exec_test 'export TEST=BLA ; echo "$TEST"'
  exec_test 'export TEST=BLA ; echo "$=TEST"'
  exec_test 'export TEST=BLA ; echo "$ "'
  exec_test 'export TEST=BLA ; echo "$ f"'
  exec_test 'export TEST=BLA ; echo " $ "'
  exec_test 'export TEST=BLA ; echo " $ $USER"'
  exec_test 'export TEST=BLA ; echo ""$ '
  exec_test 'export TEST=BLA ; echo $"" '
  exec_test 'export TEST=BLA ; echo $'' '
  exec_test 'export TEST=BLA ; echo X'"$"""''
  exec_test 'export TEST=BLA ; echo "$?TEST"'
  exec_test 'export TEST=BLA ; echo $TEST $TEST'
  exec_test 'export TEST=BLA ; echo "$1TEST"'
  exec_test 'export TEST=BLA ; echo "$T1TEST"'
  exec_test 'export CHEVRON=">" ; echo bla $CHEVRON yo ; ls'
  exec_test 'echo " $" ; echo "$" ; echo "$""" ; echo "$"XX"" ; echo X"$"""'
  exec_test 'echo $'
  exec_test 'echo " $ "'
  exec_test 'echo " $? "'
  exec_test 'echo $''?$""$' '$" "$'?'$"?"$'? '$"? "" $ "" $? "'
  exec_test 'echo $'USER''
  exec_test 'echo $"USER"'
  exec_test 'echo $'USER ''
  exec_test 'echo $"USER "'
  exec_test 'echo " $ "'
  exec_test 'echo " $USER "'
  exec_test 'echo $'USERR''
  exec_test 'echo $"USERR"'
  exec_test 'echo $'USERR ''
  exec_test 'echo $"USERR "'
  exec_test 'echo " $ "'
  exec_test 'echo " $USERR "'
  exec_test 'echo $''?$""$' '$" "$'USER'$"USER"$'USER '$"USER "" $ "" $USER "'
  exec_test 'echo $?$?$?$?$?$?$?$'
  exec_test 'echo $'USER''
  exec_test 'echo $"USER"'
  exec_test 'echo '$USER''
  exec_test 'echo "$USER"'
  exec_test 'echo "'$USER'"'
  exec_test 'echo '"$USER"''
  exec_test 'echo $USER"'"'
  exec_test 'echo $USER""'
  exec_test 'echo ''$USER'
  exec_test 'echo $'USER'$"USER"'$USER'"$USER""'$USER'"'"$USER"'$USER"'"$USER""''$USER'
  exec_test 'echo $?$USER'
  exec_test 'export 'USER'=YO'
  exec_test 'echo $'USER''
  exec_test 'echo $'''
  exec_test 'echo $""'
  exec_test 'echo $' ''
  exec_test 'echo $" "'
  exec_test 'echo $'?''
  exec_test 'echo $"?"'
  exec_test 'echo $'? ''
  exec_test 'echo $"? "'
  exec_test 'echo " $"USER'
  exec_test 'export var="truc" ; echo $var'
  exec_test 'export var="truc" ; echo $var | cat -e'
  exec_test 'echo "$tests""Makefile"'
  exec_test 'echo "$tests"Makefile'
  exec_test 'echo "$tests" "Makefile"'
  exec_test '$bla'
  exec_test 'export var="truc" ; echo $var bonjour ; export $var'
  exec_test 'export test=123'
  exec_test 'export var= s -la ; l$var'
  exec_test 'export var=at ; c$var Makefile'
  exec_test 'export loop='bonjour$loop' ; echo $loop'
fi

# EXPORT
if [ "$1" == "export" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tEXPORT TESTS\n"$RESET
  ENV_SHOW="env | sort | grep -v SHLVL | grep -v _="
  EXPORT_SHOW="export | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
  exec_test 'export ='
  exec_test "export 1TEST= ; $ENV_SHOW"
  exec_test "export TEST ; $EXPORT_SHOW"
  exec_test "export ""="" ; $ENV_SHOW"
  exec_test "export TES=T="" ; $ENV_SHOW"
  exec_test "export TE+S=T="" ; $ENV_SHOW"
  exec_test "export TEST=LOL ; echo $TEST ; $ENV_SHOW"
  exec_test "export TEST=LOL ; echo $TEST$TEST$TEST=lol$TEST"
  exec_test "export TEST1=LOL TEST2=PIKAPIKA ; echo $TEST ; $ENV_SHOW"
  exec_test "export TEST1=LOL TEST2 $ENV_SHOW"
  exec_test "export TEST=LOL ; unset TEST $ENV_SHOW"
  exec_test "export TEST=LOL ; export TEST+=LOL ; echo $TEST ; $ENV_SHOW"
  exec_test $ENV_SHOW
  exec_test $EXPORT_SHOW
  exec_test "export TEST="ls -l - a" ; echo $TEST ; $ENV_SHOW"
  exec_test 'export TEST=BLA ; echo test        test'
  exec_test 'unset _ ; env | sort'
  exec_test 'export TESTFCGFCSGFC ; env | sort'
  exec_test 'export TESTNBKJNLJ='rferfh$ref' ; env | sort'
  exec_test 'export TESTNBKJNLJdgf="'rferfh$ref'" ; env | sort'
  exec_test 'export TESTNBKJNLJtcfgvhjbk="rferfh$ref" ; env | sort'
  exec_test 'export = ; env | sort'
  exec_test 'export 1TESTHBGUHBUHB= ; env | sort'
  exec_test 'export TESTOUHOSIOUNXJKNK 34khh -deded ; env | sort'
  exec_test 'export ""="" ; env | sort'
  exec_test 'export "=" ; env | sort'
  exec_test 'export TESTFGCHVJKNLJKHVJGCHFGVJBKNL2='' ; env | sort'
  exec_test 'export TESTFGCHVJKNLJKHVJGCHFGVJBKNL3="" ; env | sort'
  exec_test 'export "$" ; env | sort'
  exec_test 'export TESTJHBSJCBJHSC111=T="" ; env | sort'
  exec_test 'export TESTNJNCKJDCKDCKDJN222+S=T="" ; env | sort'
  exec_test 'export TESTKXSLXSLX333=LOL ; env | sort'
  exec_test 'export TESTYGVSBNJKHBXGJHN444=LOL ; env | sort'
  exec_test 'export TESTREDTUHINJO555=LOL ; env | sort'
  exec_test 'export TESTYRFTGUIHUJOKL666=lol ; env | sort'
  exec_test 'export TESTYRFTGUIHUJOKL666=LOLdfgtgh ; env | sort'
  exec_test 'export TESTYTFUGYHIUNJ777="ls       -l     - a" ; env | sort'
  exec_test 'export | grep TEST ; env | sort'
  exec_test 'export arg='"hello"' ; env | sort'
  exec_test 'export TESTFGCHVJKNLJKHVJGCHFGVJBKNL4="$" ; env | sort'
  exec_test 'export TESTrsxydtcfyvgubhnijomkonjbhiugv=bla ; export TESTtdcvhbknlm="YO" ; export TESTjknmlmklnj='kuhkuhk' ; export TESTjkndfv='"kuhku"hk' ; export TESTjkndfvnj='"ku$?hku"hk' ; export | grep TEST ; unset TESTtdcvhbknlm | unset TESTjknmlmklnj ; export | grep TEST ; unset TESTtdcvhbknlm | TESTjknmlmklnj ; export | grep TEST ; unset   TESTjkndfv='"kuhku"hk' ; export | grep TEST ; unset   _TESTjkndfv= ; export | grep TEST ; unset   __TESTjkndfv= ; export | grep TEST ; unset 0 ; export | grep TEST ; unset   567890RFUYGVHBK | unset 65798uhjnk ; export | grep TEST ; unset       "TESTrsxydtcfyvgubhnijomkonjbhiugv" ; export | grep TEST'
  exec_test 'unset PATH ; echo $PATH ; ls ; unset "" test ; unset = ; unset PWD ; echo "END"'
  exec_test 'echo $test ; export test="  foo    bar  " ; echo $test ; export test="  foo    bar  " ; echo ab$test ; export test="  foo    bar  " ; echo "ab"$test ; export test=" foo   bar " ; echo ab"$test" | cat -e ; export test=" foo   bar " ; echo "$test" ; export test=" foo   bar " ; echo ""$test"" ; export test=" foo   bar " ; echo """$test"""'
  exec_test 'export _TEST=tewfqq ; export'

fi

# REDIRECTIONS
if [ "$1" == "redirect" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tREDIRECTION TESTS\n"$RESET
  exec_test 'echo test > ls ; cat ls'
  exec_test 'echo test > ls >> ls >> ls ; echo test >> ls ; cat ls'
  exec_test '> lol echo test lol ; cat lol'
  exec_test '>lol echo > test>lol>test>>lol>test mdr >lol test >test ; cat test'
  exec_test 'cat < ls'
  exec_test 'rm -f ls ; cat > ls < ls ; rm -f ls'
  exec_test 'ls > ls'
  exec_test 'cat <ls'
  exec_test 'pwd >pwd ; cat pwd'
  exec_test 'pwd >pwd ; cat pwd | echo'
  exec_test 'cat | <Makefile cat ; hello'
  exec_test 'cat <test.sh <ls'
  exec_test 'cat << stop ; 1 ; stop'
  exec_test 'cat << stop ; 1OF ; stopa ; stop'
  exec_test 'cat <test.sh <<stop ; 1 ; stop'
  exec_test 'cat <<stop<ls ; 1 ; stop'
  exec_test 'cat <test.sh << stop1 <<stop2 ; a ; b ; c ; stop1 ; run2 ; stop2'
  exec_test 'rm -f ls >ls'
  exec_test 'echo jhjbsdc >> tmp1 ; cat tmp1 ; ls ; cat tmp1 ; rm tmp1'
  exec_test 'echo kjnsdc sdc sddc > tmp1 knsdc sdc  sdcsdc > tmp1 ; cat tmp1 ; ls ; cat tmp1 ; rm tmp1'
  exec_test 'cat < tester.sh < jnsdc ssdjcnnj > tmp2 ; ls ; cat tmp2 ; rm tmp2'
  exec_test 'cat tmp2 ; cat < tester.sh > tmp2 ; ls ; cat tmp2 ; rm tmp2'
  exec_test 'cat<tester.sh<jnsdc ssdjcnnj>tmp2 ; ls ; cat tmp2 ; rm tmp2'
  exec_test 'ls >tmp1 > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'ls >tmp1 >> tmp1 >>tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'ls >tmp1 > tmp1 <tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'echo hello > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'ls >tmp1 >> tmp1 >>tmp1 | cat ; rm tmp1'
  exec_test 'echo hello > tmp1 ; echo hhell >> tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'echo hello >> tmp1 | echo bla ; cat tmp1 ; rm tmp1'
  exec_test 'echo hello >> tmp1 | echobla ; cat tmp1 ; rm tmp1'
  exec_test 'echobla | echo hello >> tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'cat << '$USER' > tmp1 ; ; cat tmp1 ; rm tmp1'
  exec_test 'cat << $USER > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'cat << $AAAAAAAA > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'cat << "hello'ksdcksd'ckdsc'""""""'"kjbsdckj'"fddvdf"vdfvdfv'bsdc > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'cat << "b$" > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test '<a cat <b <c ; <a cat <b <c ; > test1 | echo blabla ; cat hello > test1 ; >test1 cat <test2 >>test3 ; >a ls <machin >>c >d ; ls ; >test1 ; cat test1 ; rm test1 test2 test3 d a'
fi


# MULTI TESTS
if [ "$1" == "multi" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tMULTI TESTS\n"$RESET
  exec_test 'echo testing multi >lol ; echo <lol <lola ; echo "test 1  | and 2" >>lol ; cat <lol ; cat ../Makefile <lol | grep minishell'
  exec_test 'unset PATH ; /bin/ls'
  exec_test 'unset PATH ; ./Makefile'
  exec_test 'echo 5 > ls ; <5 cat ; rm 5'
  exec_test 'ls | echo 6 > ls ; <6 cat ; rm 6'
  exec_test 'cd ; unset HOME ; cd'
  exec_test 'cd .. > 1 ; pwd'
  exec_test 'cd .. > 1| pwd'
  exec_test 'pwd > 1'
  exec_test 'pwd > 1 ; cat 1'
  exec_test 'pwd > 1 ; pwd'
  exec_test 'pwd > 1| pwd'
  exec_test 'pwd ; unset HOME ; pwd ; cd ; pwd'
  exec_test 'ls | export TEST=5 ; echo $TEST'
  exec_test 'export TEST1=LOL TEST2=PIKAPIKA ; unset TEST1 TEST2 ; echo $TEST1 ; echo $TEST2'
  exec_test '< tmp1 | > tmp1 ; ls ; rm tmp1'
  exec_test '< $SAMEYEE > tmp1 ; ls ; rm tmp1'
  exec_test 'echo hello > tmp1 ; cat < tmp1 | cat < tmp2 ; ls ; /bin/lsa ; rm tmp1'
  exec_test 'cat < tester.sh | cat < bla | echo hello > tmp2 ; cat tmp2 ; ls ; bla ; rm tmp2 ; chmod 000 tmp2 ; cat tester.sh >> tmp1 > tmp2 > tmp3 | cat tester.sh > tmp1 > tmp3 ; ls ; cat tmp3 ; rm tmp1 tmp2 tmp3'
  exec_test 'echo oui | cat -e ; echo oui | echo non | echo something | grep oui ; echo oui | echo non | echo something | grep non ; echo oui | echo non | echo something | grep something ; cd .. | echo "something" ; cd .. | echo "something" ; cd / | echo "something" ; cd .. | pwd'
  exec_test 'ifconfig | grep ":"'
  exec_test 'ifconfig | grep nothing'
  exec_test 'whoami | grep $USER ; cat tmp1 ; rm tmp1'
  exec_test 'whoami | grep $USER > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'whoami | cat -e | cat -e > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'whoami | grep "$USER'c'" > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'whoami | grep "$USER''" > tmp1 ; cat tmp1 ; rm tmp1'
  exec_test 'export TEST=123 | cat -e | cat -e ; unset TEST | cat -e ; echo test | cat -e | cat -e | cat -e'
  exec_test 'whereis ls | cat -e | cat -e > tmp2 ; cat tmp2 ; rm tmp2'
  exec_test 'echo test | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e'
  exec_test 'ls  | grep "."'
  exec_test 'whereis grep > tmp1 ; whereis grep > tmp2 ; cat tmp1 ; cat tmp2 ; ls  > tmp1 ; ls  < tmp1 ; cat tmp1 ; unset PATH ; ls | ls | ls ; rm tmp1 ; rm tmp2'
  exec_test 'echo bla | exit'
  exec_test 'echo bla | exit khbsdk'
  exec_test 'cat < notexist | exit 42'
  exec_test 'exit 42 | cat < notexist'
  exec_test 'exit tamere | exit'
  exec_test 'exit | exit  1| exit 200'
  exec_test '> test1 echo bla ; >> test1 echo bla ; > test1 << pwd ; bla ; pwd ; cat test1 ; rm test1'
  exec_test 'cd random_cmd'
  exec_test 'cd random_cmd'
  exec_test 'cd $?'
  exec_test './file_that_is_not_an_executable'
  exec_test 'cat bla	1'
  exec_test 'file_name'
  exec_test 'not_cmd'
  exec_test 'ls bonjour'
  exec_test 'export test=a'
  exec_test 'echo $test'
#  exec_test '<test1 | <test2'
fi

# SYNTAX 
if [ "$1" == "syntax" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tSYNTAX\n"$RESET
  exec_test ''
  exec_test '" echo" lol'
  exec_test '"echo " lol'
  exec_test '" echo" | " pwd"'
  exec_test '"echo " | "pwd "'
  exec_test '/bin/lsa'
  exec_test './Makefile'
  exec_test '| test'
  exec_test 'echo > <'
  exec_test 'echo | |'
  exec_test 'echo "||"'
  exec_test '<'
  exec_test 'rm -f ls ; cat < ls > ls'
  exec_test 'grep -z'
  exec_test 'ls"| "wc -l'
  exec_test '/ls'
fi

# SIGNALS
if [ "$1" == "signal" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tSIGNALS\n"$RESET
  exec_test 'kill -INT 888'
  exec_test 'echo hello | kill -INT 888'
fi

# EXIT
if [ "$1" == "exit" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tEXIT\n"$RESET
  exec_test 'exit 42'
  exec_test 'exit 42 53 68'
  exec_test 'exit 259'
  exec_test 'exit 9223372036854775807'
  exec_test 'exit -9223372036854775808'
  exec_test 'exit 9223372036854775806'
  exec_test 'exit -9223372036854775807'
  exec_test 'exit 9223372036854775808'
  exec_test 'exit 9223372036854775809'
  exec_test 'exit -9223372036854775809'
  exec_test 'exit -9223372036854775810'
  exec_test 'exit 9223372037754775807'
  exec_test 'exit -9223372037754775808'
  exec_test 'exit 99999999999999999999999'
  exec_test 'exit -99999999999999999999999'
  exec_test 'exit -4'
  exec_test 'exit wrong'
  exec_test 'exit  2 wrong_command'
  exec_test 'gdagadgag'
  exec_test 'ls -Z'
  exec_test 'cd gdhahahad'
  exec_test 'ls -la | wtf'
  exec_test 'exit 0 0'
  exec_test 'exit 42 42'
  exec_test 'exit -42 -24'
  exec_test 'exit 42'
  exec_test 'exit 42 53 68'
  exec_test 'exit 259'
  exec_test 'exit -12030'
  exec_test 'exit --1239312'
  exec_test 'exit ++++1203020103'
  exec_test 'exit +0'
  exec_test 'exit ++++++0'
  exec_test 'exit -----0'
  exec_test 'exit azerty'
  exec_test 'exit kewkwqke'
  exec_test 'exit a'
  exec_test 'exit z'
  exec_test 'exit "1"'
  exec_test 'exit "2"'
  exec_test 'exit "+102"'
  exec_test 'exit "1230"'
  exec_test 'exit "+++1230"'
  exec_test 'exit "1"23'
  exec_test 'exit "2"32"32"'
  exec_test 'exit "'42'"'
  exec_test 'exit '42'"42"42'
  exec_test 'exit +'42'"42"42'
  exec_test 'exit -'42'"42"42'
  exec_test 'exit 9223372036854775807'
  exec_test 'exit 9223372036854775808'
  exec_test 'exit 9223372036854775806'
  exec_test 'exit 9223372036854775809'
  exec_test 'exit -9223372036854775807'
  exec_test 'exit -9223372036854775808'
  exec_test 'exit -9223372036854775806'
  exec_test 'exit -9223372036854775809'
  exec_test 'exit -4'
  exec_test 'exit wrong'
  exec_test 'exit wrong_command'
  exec_test 'exit something'
  exec_test 'exit 1'
  exec_test 'exit -1'
  exec_test 'exit 42'
  exec_test 'exit 0'
  exec_test 'exit --000'
  exec_test 'exit +++++++000'
  exec_test 'exit ++++3193912939'
  exec_test 'exit ---31232103012'
  exec_test 'exit "something"'
  exec_test 'exit q'
  exec_test 'exit help'
  exec_test 'exit export'
  exec_test 'exit echo'
  exec_test 'exit cd ..'
  exec_test 'exit 0 0'
  exec_test 'exit something somethingv2'
  exec_test 'exit 42 42 42 42 42'
  exec_test 'exit echo something'
  exec_test 'exit exit'
fi

# BONUS
if [ "$1" == "bonus" ] || [ "$1" == "quote" ] || [ "$1" == "wildcard" ] || [ "$1" == "oper" ]; then
  Compile and set executable rights
  make bonus -C ../ > /dev/null
  cp ../minishell .
  chmod 755 minishell
fi

# BONUS WILDCARD
if [ "$1" == "bonus" ] || [ "$1" == "wildcard" ]; then
  printf $BOLDMAGENTA"\n\tBONUS WILDCARD\n"$RESET
  exec_test "echo * | wc"
  exec_test "cd ..  ; echo * | wc"
  exec_test "echo .* | wc"
  exec_test "echo M*e"
  exec_test "echo *a*e"
  exec_test "echo *.mp3"
  exec_test "mkdir empty ; cd empty ; pwd ; echo * ; cd .. ; rm -rf empty"
fi

# BONUS OPERATOR && || ()
if [ "$1" == "bonus" ] || [ "$1" == "oper" ]; then
  printf $BOLDMAGENTA"\n\tBONUS OPERATOR \$\$ || () \n"$RESET
  exec_test "true && ls"
  exec_test "false&&ls"
  exec_test "true||ls"
  exec_test "false || ls"
  exec_test "true || echo 1 && echo 2"
  exec_test "false || echo 1 && echo 2"
  exec_test "true || (echo 1 && echo 2)"
  exec_test "true || echo 1 && echo 2 || echo 3"
  exec_test "(ls)"
  exec_test "( ls )"
  exec_test "true || (echo 1 && echo 2) || echo 3"
  exec_test "true || (echo 1 && echo 2) && echo 3"
  exec_test "(true || (echo 1 && echo 2) && echo 3)"
  exec_test "true || ((echo 1 && echo 2) && echo 3)"
  exec_test "( )"
  exec_test " ls )"
  exec_test "( ls " 
fi

if [[ "$1" != "" ]] && (( $TOTAL > 0)); then
  PROCENT=$(( $GOOD * 100  / $TOTAL ))
  if (( $PROCENT > 90)); then
    printf $GREEN
  elif (( $PROCENT <= 90 )) && (( $PROCENT > 60 )); then
    printf $YELLOW
  else  
    printf $RED
  fi
  printf "\nPASS: $GOOD / $TOTAL ($PROCENT%%)$RESET\n"
fi

rm -f $pipe lol ls 1 test big_file msh_log leaks pwd test1 tmp1 tmp2 minishell
