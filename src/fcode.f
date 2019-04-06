      subroutine fortran_function(f1, f2, f3, f4)
        integer*1 f1
        integer*2 f2
        integer*4 f3
        integer*4 f4

        write (*,100)f1,f2,f3,f4
  100   format(4i5)
      end subroutine fortran_function
