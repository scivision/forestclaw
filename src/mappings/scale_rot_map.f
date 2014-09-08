c     # ----------------------------------------------
c     # Scaling routines
c     # ----------------------------------------------
      subroutine set_scale(scale)
      implicit none
      double precision scale, scale_com

      common /comscale/ scale_com
      scale_com = scale

      end


      double precision function get_scale()
      implicit none
      double precision scale_com
      common /comscale/ scale_com

      get_scale = scale_com

      end

      subroutine scale_map(xp,yp,zp)
      implicit none

      double precision xp,yp,zp
      double precision s, get_scale

      s = get_scale()

      xp = s*xp
      yp = s*yp
      zp = s*zp

      end


c     # ----------------------------------------------
c     # Rotation routines
c     # ----------------------------------------------
      subroutine set_rotation(rot_angle)
      implicit none

      double precision rot_angle(2), scale

      double precision rrot(3,3), r1(3,3), r2(3,3)

      double precision th, phi
      integer i,j,k

      double precision rrot_com(3,3)
      common /comrot/ rrot_com

c     # Rotates map so as to not bias the solution.
      do i = 1,3
         do j = 1,3
            r1(i,j) = 0
         enddo
         r1(i,i) = 1
      enddo

      th = rot_angle(1)
      r1(1,1) = cos(th)
      r1(1,2) = -sin(th)
      r1(2,1) = -r1(1,2)
      r1(2,2) = r1(1,1)

c     # rotate in second direction (for full 3d map)
      do i = 1,3
         do j = 1,3
            r2(i,j) = 0
         enddo
         r2(i,i) = 1
      enddo
      phi = rot_angle(2)
      r2(1,1) = cos(phi)
      r2(1,3) = -sin(phi)
      r2(3,1) = -r2(1,3)
      r2(3,3) = r2(1,1)
      do i = 1,3
         do j = 1,3
            rrot(i,j) = 0
            do k = 1,3
               rrot(i,j) = rrot(i,j) + r2(i,k)*r1(k,j)
            enddo
         enddo
      enddo

      do j = 1,3
         do i = 1,3
            rrot_com(i,j) = rrot(i,j)
         enddo
      enddo

      end


      subroutine get_rotation(rrot)
      implicit none

      double precision rrot(3,3), rrot_com(3,3)
      common /comrot/ rrot_com

      integer i,j

      do j = 1,3
         do i = 1,3
            rrot(i,j) = rrot_com(i,j)
         enddo
      enddo

      end


      subroutine rotate_map(xp,yp,zp)
      implicit none

      double precision xp,yp,zp
      double precision v(3), vrot(3)
      integer i,k
      double precision rrot(3,3)

      v(1) = xp
      v(2) = yp
      v(3) = zp

      call get_rotation(rrot)

      do i = 1,3
         vrot(i) = v(i)
      enddo

c     # Rotate mapping
      do i = 1,3
         vrot(i) = 0
         do k = 1,3
            vrot(i) = vrot(i) + rrot(i,k)*v(k)
         enddo
      enddo

      xp = vrot(1)
      yp = vrot(2)
      zp = vrot(3)

      end


c     # ----------------------------------------------
c     # Shift routines
c     # ----------------------------------------------

      subroutine set_shift(shift)
      implicit none

      double precision shift(3), shift_com(3)
      integer m

      common /comshift/ shift_com

      do m = 1,3
         shift_com(m) = shift(m)
      enddo


      end

      subroutine get_shift(shift)
      implicit none

      double precision shift(3), shift_com(3)
      integer m

      common /comshift/ shift_com

      do m = 1,3
         shift(m) = shift_com(m)
      enddo


      end

      subroutine shift_map(xp,yp,zp)
      implicit none

      double precision xp,yp,zp
      double precision shift(3)
      integer m


      call get_shift(shift)

      xp = xp + shift(1)
      yp = yp + shift(2)
      zp = zp + shift(3)
      end



c      subroutine setup_mappedgrid(rot_angle,scale)
c      implicit none
c      double precision rot_angle(2), scale
c
c      call set_scale(scale)
c      call set_rotation(rot_angle)
c
c      end
c
c      subroutine set_map_defaults()
c      implicit none
c      double precision rot_angle(2), scale
c
c      rot_angle(1) = 0.d0
c      rot_angle(2) = 0.d0
c      call set_rotation(rot_angle)
c
c      scale = 1.d0
c      call set_scale(scale)
c
c      end
