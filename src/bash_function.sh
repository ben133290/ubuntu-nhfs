#TODO:doesn't recognize '' in string yet. what could be the solution?
#TODO: after function ran return to current directory and only then display information
echo 'nhfs()
      {
          cd /home/seraina/ubuntu-nhfs/bin/ #needs to be dynamic (setup.c) depending on where it will be saved.
          if $1 == ''
          then
          	./tfs
          elif $2 == ''
          then
      	./tfs $1
      	elif $3 == ''
      	then
      	./tfs $1 $2
      	elif $4 == ''
          	then
          	./tfs $1 $2 $3
          else
      	./tfs $1 $2 $3 $4
          fi
      }' >> ~/.bashrc
source ~/.bashrc