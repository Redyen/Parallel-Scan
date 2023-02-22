echo =======================================
echo Times and Validation checks are 
echo inside respective textfiles
echo =======================================
echo ---------Make----------
make
echo
echo ---------Serial has started----------
echo
./scan
echo
echo ---------OpenMP has started----------
echo
./scan_omp
echo
echo ---------MPI has started----------
echo 
mpirun -np 2 ./scan_mpi
echo
echo ---------Finished----------