      SUBROUTINE fortran_function(f1, f2, f3, f4)
        IMPLICIT NONE
        INTEGER*1 f1
        INTEGER*2 f2
        INTEGER*4 f3
        INTEGER*4 f4

        WRITE (*,100)f1,f2,f3,f4
  100   FORMAT(4i5)
      END SUBROUTINE fortran_function
