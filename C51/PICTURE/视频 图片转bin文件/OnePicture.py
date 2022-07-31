import cv2
import os
import struct

def pictureBinary(name):
    filename='./'+ name +'.bmp'
    if not os.path.isfile(filename):
        img=cv2.imread("./"+name+".jpg",0)
        img_shape=img.shape
        width=int(img_shape[0]*(64.0/img_shape[1]))
        height=64
        newImg=cv2.resize(img,(128,64))
        (ret,th)=cv2.threshold(newImg,127,255,cv2.THRESH_BINARY)
        cv2.imwrite(filename,th)
        print(filename)
    else:
        print(filename+"已存在")

def createBin(name):
    filename="./"+name+".bmp"
    img=cv2.imread(filename)
    file =  open("./"+name+".bin",'ab')
    for height in range(0,64):
        container=0
        count=0
        for width in range(0,128):
            count+=1
            if count==8:
                container=(container+int(bool(img[height,width][1])))
            if count<8:
                container=(container+int(bool(img[height,width][1])))<<1
            else:    
                s = struct.pack('B',int(container))#转换为字节流字符串，B代表unsigned char
                file.write(s)
                print(hex(int(container))[2:].zfill(2),end="")
                count=0
                container=0
                
        print("")
    #os.system("cls")
    
if __name__=="__main__":
    filename=input("输入文件名(去掉.jpg后缀)")
    pictureBinary(filename)
    createBin(filename)