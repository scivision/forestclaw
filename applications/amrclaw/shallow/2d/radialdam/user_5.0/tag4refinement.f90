SUBROUTINE clawpack5_tag4refinement(mx,my,mbc,meqn, &
           xlower,ylower,dx,dy,blockno, q,refine_threshold, &
           init_flag, tag_patch)
  IMPLICIT NONE

  INTEGER mx,my, mbc, meqn, tag_patch, init_flag
  INTEGER blockno
  DOUBLE PRECISION xlower, ylower, dx, dy
  DOUBLE PRECISION refine_threshold
  DOUBLE PRECISION q(meqn,1-mbc:mx+mbc,1-mbc:my+mbc)

  INTEGER i,j, mq,m
  DOUBLE PRECISION xc,yc, qmin, qmax
  DOUBLE PRECISION dq, dqi, dqj

  tag_patch = 0

  !! # Refine based only on first variable in system.
  mq = 1
  DO i = 1,mx
     DO j = 1,my
        qmin = MIN(q(mq,i,j),qmin)
        qmax = MAX(q(mq,i,j),qmax)
        IF (qmax - qmin .GT. 0.2) THEN
           tag_patch = 1
           RETURN
        ENDIF

!!         IF (init_flag .NE. 0) THEN
!!            IF (ABS(q(mq,i,j)) .GT. 1.1) THEN
!!               tag_patch = 1
!!               RETURN
!!            END IF
!!         ELSE
!!            IF (ABS(q(mq,i,j)) .GT. refine_threshold) THEN
!!               tag_patch = 1
!!               RETURN
!!            ENDIF
!!         END IF
     ENDDO
  ENDDO

END SUBROUTINE clawpack5_tag4refinement
