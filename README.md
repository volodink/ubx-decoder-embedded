### Формат выдачи

```
AlatBlonCheightDspeedEsatallFsatsolGfixtype\n
```

lat      - широта

lon      - долгота

height   - высота

speed    - скорость

vAcc - вертикальная точность 

hAcc - горизонтальная точность

pAcc - 3D точность

satall - количество спутников (все)

satsol - количество спутников (в решении)

fixtype - тип Fix'a (0-все плохо, 5 - не бывает)


### Для работы с проектом из CLion:

Установить CLion

```
http://www.jetbrains.com/clion/ 
```

Установить Platformio

```
sudo apt-get install python-dev python-pip
sudo pip install -U platformio
platformio platforms install atmelavr
platformio lib install 13
platformio lib install 81
platformio lib install 73
platformio lib install 433
```

В папке ide/clion выполнить:

```
platformio init -b nanoatmega328 --ide clion
```

после чего открыть нормальными средствами среды.

Собрать проект:

```
platformio run
```

Загрузить на платформу

```
platformio run -t upload
```

