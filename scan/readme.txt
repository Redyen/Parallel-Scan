(1) ./run.sh will compile the 3 programs 
(2) Serial and openMP will be timed and validated on inputs of size 2^(p) where p increments in sizes of 3 between 3-18
(3) Times of each run will be saved un Times.txt and their validation check on size 2^(3) will be displayed in results
(4) Larger than 2^(3) inputs are validated by ensure their lists are ascending 
(5) I was unable to dynamically increment MPI as i did with Serial and openMP thus it is manually adjusted, running ./run.sh will
    only run of size 2^15. however all increments were timed and validated for the report manually 