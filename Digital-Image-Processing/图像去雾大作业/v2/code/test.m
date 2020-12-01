filename = ["1.bmp", "2.bmp", "3.jpg", "4.bmp", "5.bmp", "6.bmp", "7.bmp", "8.bmp", "9.bmp"];
ResBase = "../res/";

for i = 1 : 1 : 9
    ImagePath = strcat("../raw/", filename(i));
    J = Dehaze(ImagePath);
    ResPath = strcat(strcat(ResBase, num2str(i)), ".png");
    imwrite(J, ResPath);
end
