import array
import ctypes
import time

def loop():
    while True:
        try:
            i = int(input("enter level: "))
        except:
            print("error: invalid entry\n")
            loop()

        if i < 1 or i > 255:
            print("error: invalid entry\n")
            loop()

        hex(i)
        level = ctypes.c_ubyte(i)
        try:
            with open("ps_game.exe", "r+b") as f:
                #raw offsets
                a = array.array("i", [
                    0x60057, 0x60087, 0x600B7,
                    0x606CE, 0x606F9, 0x60724,
                    0x6075D, 0x607AE, 0x607FB,
                    0x60840, 0x643F7, 0x64480,
                    0x64561, 0x645DC, 0x645EC,
                    0x64641, 0x66FFE, 0x8020E,
                    0x803C6, 0x9181D, 0x9183D,
                    0x9A8A4, 0x9A8D5, 0x9A906,
                    0x9A9E0, 0x9AA3C, 0x9AA90,
                    0x9ABC4, 0x9AC20, 0x9AC74,
                    0x9AF77, 0x9AF9E, 0x9B1A0,
                    0x9B1D1, 0x9B202, 0x9B2DD,
                    0x9B336, 0x9B383, 0x9B3D3,
                    0x9B415, 0x9B454, 0x9B4B6,
                    0x9B50F, 0x9B55C, 0x9B5AC,
                    0x9B5EE, 0x9B62D, 0x14641B  
                ])

                for e in a:
                    f.seek(e)
                    f.write(level)

                f.close()

        except:
            print("error: file not found\n")
            loop()

        else:
            print("file processed") 
            time.sleep(1.0)
            exit(0)
#main
loop()
