package main

import (
  "fmt"
  "time"
  "os"
  "strconv"
  "sync"
)

type Matrix []float64

func Min(x, y int) int {
 if x < y {
   return x
 }
 return y
}

func PrintMatrix(w int, h int, m *Matrix) {
 for i:=0; i<w; i++ {
  for j:=0; j<h; j++ {
    fmt.Printf("%f ", (*m)[i*w + j])
  }
  fmt.Println()
 }
}

func OnMult(m_ar, m_br int) {

 pha := make(Matrix, m_ar * m_ar)
 phb := make(Matrix, m_ar * m_ar)
 phc := make(Matrix, m_ar * m_ar)

 for i:=0; i < m_ar; i++ {
  for j:=0; j < m_ar; j++ {
    pha[i*m_ar + j] = 1.0
  }
 }

 for i:=0; i < m_br; i++ {
  for j:=0; j < m_br; j++ {
    phb[i*m_br + j] = float64(i+1)
  }
 }

 /*fmt.Println("A Matrix:")
 PrintMatrix(m_ar, m_ar, &pha)
 fmt.Println("B Matrix:")
 PrintMatrix(m_br, m_br, &phb)*/

 start := time.Now()

 for i:=0; i < m_ar; i++ {
  for j:=0; j < m_br; j++ {
    for k:=0; k < m_ar; k++ {
      phc[i*m_ar + j] += pha[i*m_ar + k] * phb[k*m_br + j]
    }
  }
 }

 elapsed := time.Since(start)

 fmt.Println("Time(OnMult): ", elapsed)
 /*fmt.Println("Result Matrix:")
 PrintMatrix(m_ar, m_br, &phc)*/
}

func OnMultLine(m_ar, m_br int) {

 pha := make(Matrix, m_ar * m_ar)
 phb := make(Matrix, m_ar * m_ar)
 phc := make(Matrix, m_ar * m_ar)

 for i:=0; i < m_ar; i++ {
  for j:=0; j < m_ar; j++ {
    pha[i*m_ar + j] = 1.0
  }
 }

 for i:=0; i < m_br; i++ {
  for j:=0; j < m_br; j++ {
    phb[i*m_br + j] = float64(i+1)
  }
 }

 start := time.Now()

 for i:=0; i < m_ar; i++ {
  for j:=0; j < m_br; j++ {
    for k:=0; k < m_ar; k++ {
      phc[i*m_ar + k] += pha[i*m_ar + j] * phb[j*m_br + k]
   }
  }
 }

 elapsed := time.Since(start)

 fmt.Println("Time(OnMultLine): ", elapsed)
}

func OnMultLineBlock(m_ar, m_br, b, t int) {

  pha := make(Matrix, m_ar * m_ar)
  phb := make(Matrix, m_ar * m_ar)
  phc := make(Matrix, m_ar * m_ar)

  for i:=0; i < m_ar; i++ {
    for j:=0; j < m_ar; j++ {
      pha[i*m_ar + j] = 1.0
    }
  }

  for i:=0; i < m_br; i++ {
    for j:=0; j < m_br; j++ {
      phb[i*m_br + j] = float64(i+1)
    }
  }

  
  var wg sync.WaitGroup
  wg.Add(t)

  start := time.Now()

  for i := 0; i < t; i++ {
    go func(tn int) {
      OnMultLineBlockProd(m_ar, m_br, b, t, tn, pha, phb, phc)
      wg.Done()
    }(i)
  }

  wg.Wait()

  elapsed := time.Since(start)
  fmt.Println("Time(OnMultLineBlock): ", elapsed)
  
  /*fmt.Println("Result Matrix:")
  PrintMatrix(m_ar, m_br, &phc)*/
}

func OnMultLineBlockProd(m_ar, m_br, b, t, tn int, pha, phb, phc Matrix) {
  
  limBlocks := m_ar * m_ar / b
  for bn := tn; bn < limBlocks; bn += t {
    //fmt.Println("Thread:", tn, "of", t, "on block", bn)
    iIni := bn*b / m_ar
    jIni := bn*b % m_ar
    start := iIni * m_ar + jIni

    lim := start + b
    if lim >= len(pha) {
      lim = len(pha) 
    }
    //fmt.Println("Block:", bn, "Index:", start, lim, iIni, jIni)
    for index:=start; index < lim; index++ {
      i := index / m_ar
      j := index % m_ar
      for k:=0; k < m_ar; k++ {
        phc[i*m_ar + k] += pha[i*m_ar + j] * phb[j*m_br + k]
     }
    }
  }
}

func main () {
  lin := 10
  col := 10
  block := 2
  threads := 8
  opt := 3
  if len(os.Args) != 6 {
    fmt.Println("No lin, col, block size or threads values provided, defaulted to", lin, col, block, threads, ", respectively")
  } else {
    l,_ := strconv.Atoi(os.Args[1])
    c,_ := strconv.Atoi(os.Args[2])
    b,_ := strconv.Atoi(os.Args[3])
    t,_ := strconv.Atoi(os.Args[4])
    o,_ := strconv.Atoi(os.Args[5])
    lin = l
    col = c
    block = b
    threads = t
    opt = o
  }
  fmt.Println("Using lin =", lin , ", col =", col, ", block =", block, "and threads =", threads)
  switch opt {
    case 0:
      OnMult(lin,col)
    case 1:
      OnMultLine(lin,col)
    case 2:
      OnMultLineBlock(lin,col,block, threads)
    default:
      OnMult(lin,col)
      OnMultLine(lin,col)
      OnMultLineBlock(lin,col,block, threads)
  }

}

