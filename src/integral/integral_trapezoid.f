      SUBROUTINE INTEGRAL_TRAPEZOID(IVSIZ, XVEC, YVEC, FROM, TO, RESULT)
        IMPLICIT INTEGER*4 (I-N), DOUBLE PRECISION (A-H, O-Z)
        DIMENSION XVEC(IVSIZ), YVEC(IVSIZ)

        RESULT = 0
        DO 20 I = 1, IVSIZ-1
          X0 = XVEC(I)
          X1 = XVEC(I + 1)

          IF (X0.LT.FROM .OR. X1.GT.TO) GOTO 20

          Y0 = YVEC(I)
          Y1 = YVEC(I + 1)

          RESULT = RESULT + ((X1 - X0) * ((Y0 + Y1) / 2.0))
   20   CONTINUE

C       PRINT *, RESULT
      END SUBROUTINE INTEGRAL_TRAPEZOID

      FUNCTION INTERP(X, N, XV, YV)
        IMPLICIT INTEGER*4 (I-N), DOUBLE PRECISION (A-H, O-Z)
        DOUBLE PRECISION INTERP
        DIMENSION XV(N), YV(N)

        INTERP = 0

        DO 40 I = 1, N

          PROD = 1
          DO 30 J = 1, N
            IF (I.EQ.J) GOTO 30
            PROD = PROD * (X - XV(J)) / (XV(I) - XV(J))
30        CONTINUE

        INTERP = INTERP + YV(I) * PROD

40      CONTINUE

      END FUNCTION
