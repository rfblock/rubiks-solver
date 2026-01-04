for i in `seq 0 100`
do
	if ! ./bin/main "$i"; then
		exit
	else
		clear
	fi
done