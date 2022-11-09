#!/bin/bash

test_dir=./tests

for (( i = 0; i < 3; i++ ))
do
        if [ $i == 0 ]
        then
                test_file=$test_dir/256.txt
        fi

        if [ $i == 1 ]
        then
                test_file=$test_dir/1024.txt
        fi

        if [ $i == 2 ]
        then
                test_file=$test_dir/4096.txt
        fi


        for (( j = 0; j < 3; j++ ))
        do
                if [ $j == 0 ]
                then
                        test_ipc=msg
                        max_k=2
                fi

                if [ $j == 1 ]
                then
                        test_ipc=shm
                        max_k=3
                fi

                if [ $j == 2 ]
                then
                        test_ipc=pipe
                        max_k=3
                fi


                for (( k = 1; k <= max_k; k++ ))
                do
                        echo $test_file $test_ipc $k

                        for (( t = 0; t < 5; t++ ))
                        do	
                                time ./ipc $test_file $test_ipc $k
                        done
                done
        done
done
