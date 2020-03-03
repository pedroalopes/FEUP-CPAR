#!/bin/bash
date | tee -a go-output.txt
# 1 
go run matrixprod.go 600 600 0 0 0 | tee -a go-output.txt
go run matrixprod.go 1000 1000 0 0 0 | tee -a go-output.txt
go run matrixprod.go 1400 1400 0 0 0 | tee -a go-output.txt
go run matrixprod.go 1800 1800 0 0 0 | tee -a go-output.txt
go run matrixprod.go 2200 2200 0 0 0 | tee -a go-output.txt
go run matrixprod.go 2600 2600 0 0 0 | tee -a go-output.txt
go run matrixprod.go 3000 3000 0 0 0 | tee -a go-output.txt
# 2
go run matrixprod.go 600 600 0 0 1 | tee -a go-output.txt
go run matrixprod.go 1000 1000 0 0 1 | tee -a go-output.txt
go run matrixprod.go 1400 1400 0 0 1 | tee -a go-output.txt
go run matrixprod.go 1800 1800 0 0 1 | tee -a go-output.txt
go run matrixprod.go 2200 2200 0 0 1 | tee -a go-output.txt
go run matrixprod.go 2600 2600 0 0 1 | tee -a go-output.txt
go run matrixprod.go 3000 3000 0 0 1 | tee -a go-output.txt

go run matrixprod.go 4000 4000 0 0 1 | tee -a go-output.txt
go run matrixprod.go 6000 6000 0 0 1 | tee -a go-output.txt
go run matrixprod.go 8000 8000 0 0 1 | tee -a go-output.txt
go run matrixprod.go 10000 10000 0 0 1 | tee -a go-output.txt
# 3
# 128 blocks 
go run matrixprod.go 4000 4000 128 8 2 | tee -a go-output.txt
go run matrixprod.go 6000 6000 128 8 2 | tee -a go-output.txt
go run matrixprod.go 8000 8000 128 8 2 | tee -a go-output.txt
go run matrixprod.go 10000 10000 128 8 2 | tee -a go-output.txt
# 256 blocks
go run matrixprod.go 4000 4000 256 8 2 | tee -a go-output.txt
go run matrixprod.go 6000 6000 256 8 2 | tee -a go-output.txt
go run matrixprod.go 8000 8000 256 8 2 | tee -a go-output.txt
go run matrixprod.go 10000 10000 256 8 2 | tee -a go-output.txt
# 512 blocks
go run matrixprod.go 4000 4000 512 8 2 | tee -a go-output.txt
go run matrixprod.go 6000 6000 512 8 2 | tee -a go-output.txt
go run matrixprod.go 8000 8000 512 8 2 | tee -a go-output.txt
go run matrixprod.go 10000 10000 512 8 2 | tee -a go-output.txt
# 1024 blocks
go run matrixprod.go 4000 4000 1024 8 2 | tee -a go-output.txt
go run matrixprod.go 6000 6000 1024 8 2 | tee -a go-output.txt
go run matrixprod.go 8000 8000 1024 8 2 | tee -a go-output.txt
go run matrixprod.go 10000 10000 1024 8 2 | tee -a go-output.txt

date | tee -a go-output.txt
