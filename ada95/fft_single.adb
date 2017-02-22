with Text_IO, Ada.Integer_Text_IO, Ada.Long_Float_Text_IO;
use  Text_IO, Ada.Integer_Text_IO, Ada.Long_Float_Text_IO;
with Ada.Numerics, Ada.Numerics.Elementary_Functions;
use Ada.Numerics, Ada.Numerics.Elementary_Functions;

procedure fft_single is

  type Float_Vector is array (0..31) of Long_Float;

  procedure FFT(       data    :       in out Float_Vector;
                       nn      :       in integer;
                       isign   :       in integer ) is

        n, mmax, m, j, istep, i   :       integer;
        wtemp, wr, wpr, wpi, wi, theta  : Long_Float;
        tempr, tempi, tmp               : Long_Float;

     begin -- FFT

        n := nn*2;
        j := 0;
        i := 0;
        -- Bit reversal section
        while ( i < (n-1)) loop
                if ( j > i ) then
                        -- Swap real value
                        tmp := data(j);
                        data(j) := data(i);
                        data(i) := tmp;

                        -- Swap img value
                        tmp := data(j+1);
                        data(j+1) := data(i+1);
                        data(i+1) := tmp;
                end if;
                m := n/2;
                while ( ( m >= 2 ) and (j >= m )) loop
                        j := j-m;
                        m := m/2;
                end loop;

                j := j+m;
                i := i+2;
        end loop;
        mmax := 2;
        while ( n > mmax) loop

                istep := mmax*2;
                theta := ((Long_Float(isign)) * ( (2.0*Pi) / Long_Float(mmax)) );
                wtemp := Long_Float(sin(Float(0.5*theta)));
                wpr   := (-2.0*wtemp*wtemp);
                wpi   := Long_Float(sin(Float(theta)));
                wr    := 1.0;
                wi    := 0.0;
                m := 0;
                while ( m < (mmax-1) ) loop
                        i := m;
                        while ( i <= (n-1) ) loop
                                j := ( i+mmax);
                                tempr := ((wr*data(j)) - (wi*data(j+1)));
                                tempi := ((wr*data(j+1)) + (wi*data(j)));
                                data(j) := (data(i) - tempr );
                                data(j+1) := (data(i+1) - tempi);
                                data(i) := data(i) + tempr;
                                data(i+1) := data(i+1) + tempi;

                                i := i+ istep;
                        end loop;
                        wtemp := wr;
                        wr := ( (wtemp*wpr) - (wi*wpi) + wr);
                        wi := ((wi * wpr ) + (wtemp * wpi) + wi);

                        m := m+2;
                end loop;

                mmax := istep;
        end loop;

     end fft;   -- End Function FFT

        Forward :  constant integer := -1;
        Inverse :  constant integer := 1;
        vec : Float_Vector;
        i : integer;
begin
        vec(0):=1.2;
        vec(1):=0.8;
        vec(2):=2.1;
        vec(3):=2.0;
        vec(4):=3.4;
        vec(5):=0.9;
        vec(6):=7.8;
        vec(7):=1.6;
        vec(8):=7.4;
        vec(9):= 0.55;
        vec(10):=2.5;
        vec(11):=1.89;
        vec(12):=4.6;
        vec(13):=5.8;
        vec(14):=1.9;
        vec(15):=10.9;

        fft(vec, 8, Forward);

        Put_Line("*** FFT Data ***");
        for i in 0..15 loop
                Put(vec(i));
                New_Line;
        end loop;

end fft_single;
