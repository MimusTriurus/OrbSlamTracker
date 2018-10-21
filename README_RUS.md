# OrbSlamTracker

Проект основан на ORB_SLAM2 (https://github.com/raulmur/ORB_SLAM2)

[![SLAM + OpenGL](http://img.youtube.com/vi/oA76ZAMNR90/0.jpg)](http://www.youtube.com/watch?v=oA76ZAMNR90 "")

[![SLAM + Unity 5](http://img.youtube.com/vi/SGa-UU-WdEA/0.jpg)](http://www.youtube.com/watch?v=SGa-UU-WdEA "")

Системные требования:
----------------------------------------------
1. ОС			: Windows x64;
2. Версия Qt	: 5.5.1 или выше;
3. Версия OpenCV: 3.1;
4. Компилятор	: MSVC2013 x64;
5. Две Web-камеры или 1-а стереокамера.

Порядок сборки:
----------------------------------------------
1. Открыть проект в QtCreator;
2. Установить тип сборки "Выпуск";
3. Собираем проект;

Настройка:
1. В папку bin копируем библиотеки OpenCV (https://cloud.mail.ru/public/KMiB/znjPSo2CZ);
2. В папку bin копируем библиотеки Qt;
3. В папку bin копируем файлы из директории /assets (файлы: ORBvoc.txt, settings.yaml, webcam.yaml);
4. Калибруем стереопару:
 4.1 программа калибровки бинарники - https://cloud.mail.ru/public/5iHb/CbBNiNDin);
 4.2 программа калибровки исходники - https://github.com/MimusTriurus/QtStereoCamCalibration;
5. В папку bin копируем сгенерированный файл калибровки - extrinsics.yaml;
6. В файле webcam.yaml прописываем параметры матрицы камеры: Camera.fx, Camera.fy, Camera.cx, Camera.cy (соответсвенно параметры K1[0,0][1,1][0,2][1,2] в файле extrinsics.yaml)
7. В файле settings.yaml устанавливаем:
	7.1 leftCamIndex, rightCamIndex - индексы левой и правой камеры соответсвенно;
	7.2 frameWidth, frameHeight - разрешение камеры
	7.3 calibrationFilePath - путь до фала калибровки extrinsics.yaml
8. В папку bin копируем библиотеку OrbSlamTracker.dll

Примечание:
Чем меньше значение ошибки (меньше 0.5) по результатам калибровки, тем лучше работает трекер 


