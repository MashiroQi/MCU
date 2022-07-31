import cv2
import os
import struct

def videoToPictrue(videoName,speed,s):
    cap=cv2.VideoCapture(videoName+"."+s)
    isOpened=cap.isOpened
    fps=cap.get(cv2.CAP_PROP_FPS)
    width=int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height=int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    print(fps,width,height)
    vedioPictrueFolder=videoName+"_Pictrues"
    if not os.path.exists(vedioPictrueFolder):  # 是否存在这个文件夹
        os.makedirs(vedioPictrueFolder)  # 如果没有这个文件夹，那就创建一个
    i=0
    while(isOpened):
            i=i+1
            (flag,frame)=cap.read()
            if not i%speed:
                filename="./"+vedioPictrueFolder+"/"+str(i)+".jpg"
                print(filename)
                if flag==1:
                    if not os.path.isfile(filename):
                        newImg=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
                        cv2.imwrite(filename,newImg,[cv2.IMWRITE_JPEG_QUALITY,0])
                    else:
                        print("文件已存在！")
                else:
                    break
    cap.release()
    print("end!")
    return i

def pictureBinary(name,folder):
    bmpFolder=folder+"_bmp"
    picFolder=folder+"_pictrues"
    filename='./'+ bmpFolder+"/"+name+'.bmp'
    if not os.path.isfile(filename):
        img=cv2.imread("./"+picFolder+"/"+name+".jpg",0)
        img_shape=img.shape
        width=int(img_shape[0]*(64.0/img_shape[1]))
        height=64
        newImg=cv2.resize(img,(128,64))
        (ret,th)=cv2.threshold(newImg,127,255,cv2.THRESH_BINARY)
        cv2.imwrite(filename,th)
        print(filename)
    else:
        print(filename+"已存在")

def createBin(name,folder):
    binFolder=folder+"_bin"
    bmpFolder=folder+"_bmp"
    filename="./"+bmpFolder+"/"+name+".bmp"
    img=cv2.imread(filename)

    file =  open("./"+binFolder+"/"+folder+".bin",'ab')
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
    s=input("输入文件类型:")
    filename=input("输入文件名(去掉后缀):")
    speed=int(input("输入加快倍数:"))
    if not os.path.exists(filename+"_bmp"):
        os.makedirs(filename+"_bmp")  
    if not os.path.exists(filename+"_bin"):
        os.makedirs(filename+"_bin")  
    count=videoToPictrue(filename,speed,s)
    for i in range(1,count):
        if not i%speed:
            pictureBinary(str(i),filename)
    for i in range(1,count):
        if not i%speed:
            createBin(str(i),filename)
    print("转换完成!")        
    