# Хеш таблица

## Введение

Эта программа реализуете такую структуру данных как [Хеш таблица](https://ru.wikipedia.org/wiki/%D0%A5%D0%B5%D1%88-%D1%82%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D0%B0) с методом разрешения коллизий с помощью списка,
проводит сравнение разных алгоритмов хеширования по заселённости создаваемой ими хеш таблицы, а также демонстрирует 3 разных способа ассемблерных оптимизаций алгоритма поиска по хэш таблице.

В качестве элементов хеш таблицы будут выступать английские слова.

## Особенности системы

Тестирование на производительность выполнялось на процессоре **Intel I5-7300HQ**, который поддерживает SIMD инструкции стандарта AVX2 и ниже.
Это означает, что процессор может выполнять векторные инструкции над 256 битами данных одновременно. 

> [!WARNING]
> Программа вовсе не скомпилируется, если ваш процессор не поддерживает SIMD инструкций стандарта AVX2. 

## Скачивание и установка

### Скачивание

В первую очередь нужно выполнить клонирование репозитория на свой компьютер выполнив следующую команду:

``` bash
git clone --recurse-submodules https://github.com/dmitry131131/SIMD_project.git
```

### Сборка

Для сборки выполните команду:
```bash
make release
```

## Особенности тестирования производительности

В качестве инструмента тестирования будем использовать [Callgrind](https://valgrind.org/docs/manual/cl-manual.html) (Это составная часть [valgrind](https://ru.wikipedia.org/wiki/Valgrind)). И в качестве визуализатора данных будем использовать [kcachgrind](https://kcachegrind.sourceforge.net/html/Home.html).
Эта программа позволяет наглядно визуализировать деревья вызовов функций и время исполнения каждой.

В расчет будем принимать количество инструкций, выполненных в за вызов той или иной функции, а также процентное соотношение времени выполнения той или иной части программы относительно всей программы или конкретной функции.

# Исследование различных хеш функций

В этом пункте будем рассматривать **6** различных функций хеширования:

1.  Константа (return 0)
2.  ASCII код первой буквы в слове
3.  Длина слова
4.  Контрольная сумма всех ASCII кодов слова
5.  [ROR](https://ru.wikipedia.org/wiki/%D0%A1%D0%BA%D0%BE%D0%BB%D1%8C%D0%B7%D1%8F%D1%89%D0%B8%D0%B9_%D1%85%D0%B5%D1%88) хеш
6.  [CRC32](https://ru.m.wikipedia.org/wiki/%D0%A6%D0%B8%D0%BA%D0%BB%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B9_%D0%B8%D0%B7%D0%B1%D1%8B%D1%82%D0%BE%D1%87%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%B4#CRC-32) хеш

## Заселённость хеш таблицы

Для нас при выборе алгоритма хеширования ключевым ключевым пунктом является равномерность распределения данных по ячейкам хеш таблицы.

В нашем случае размер хеш таблицы был подобран так, что load-фактор составляет приблизительно 7-8 элементов на 1 ячейку таблицы. 

1. Константа

![const](img/zero.png)

2. ASCII код первой буквы

![first_alpha](img/first_alpha.png)

3.  Длина слова

![len](img/len_lash.png)

4.  Контрольная сумма всех ASCII кодов слова

![control_sum](img/control_sum.png)

5.  [ROR](https://ru.wikipedia.org/wiki/%D0%A1%D0%BA%D0%BE%D0%BB%D1%8C%D0%B7%D1%8F%D1%89%D0%B8%D0%B9_%D1%85%D0%B5%D1%88) хеш

![ror](img/ror.png)

6.  [CRC32](https://ru.m.wikipedia.org/wiki/%D0%A6%D0%B8%D0%BA%D0%BB%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B9_%D0%B8%D0%B7%D0%B1%D1%8B%D1%82%D0%BE%D1%87%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%B4#CRC-32) хеш

![crc32](img/crc32.png)

Как можно заметить лучше всего себя показывает алгоритм crc32. На графике заселённости нет ярко выраженных пиков, как на контрольной сумме и максимальное значение столбца сильно меньше, чем у ror хеша. Из чего можно сделать вывод, что из этих 6 алгоритмов лучше всего выбрать crc32.

### Контрольная сумма на малых размерах хеш таблицы

Однако не стоит считать, что контрольная сумма запрещена к использованию в качестве хэш функции.
Рассмотрим ситуацию(на этом же наборе данных) с меньшим размером хеш таблицы. Вместо продиктованного load-фактором 7-8 возьмём размер в 101 ячейку.

График заселённости для размера в 101 ячейку
![control_sum_101](img/control_sum_101.png)

График заселённости для размера в 2001 ячейку

![control_sum](img/control_sum.png)

Можно заметить, что для размера хеш таблицы в 101 ячейку контрольная сумма вполне подходящий хеш, потому что он обеспечивает некоторый приемлемый уровень равномерности.
Следовательно при работе с малыми хэш таблицами и малым объёмом данных это хеш вполне можно использовать.

Объясним теперь периодичный характер контрольной суммы на размере в 2001 ячейку. 
Дело в том что ASCII коды строчных английских букв находятся в диапазоне от 97 до 122 следовательно эти пики соответствуют словам состоящих из {1, 2, 3, 4, 5, 6, ... 33} букв. Поскольку максимальная длина слова в выборке данных 33, то хеш может "огибать таблицу 1 раз" это и создаёт шум в самом начале таблицы (в диапазоне от 0 до 97), где в принципе не может быть ни одного ASCII символа

### ROR хеш

Рассмотрим подробнее реализацию алгоритма ror хеша:

```C
size_t ror_hash(const char* str, size_t table_len)
{
    size_t temp_symbol = 0;

    for (size_t i = 0; str[i]; i++)
    {   
        temp_symbol = ((temp_symbol >> 1) | (temp_symbol << (sizeof(temp_symbol) * 8 - 1))) ^ ((size_t) str[i]);
    }

    return temp_symbol % table_len;
}
```

Фактически вся функциональная часть алгоритма - циклический сдвиг вправо, который написан с помощью побитовых операций.

Интересно увидеть как компилятор оптимизирует этот участок кода.
Запустим компилятор с флагом `-O0` и посмотрим во что скомпилируется функция

![ror_0](img/ror_O0_1.png)

Как мы видим компилятор напрямую транслировал всё, что написано на языке С

Теперь скомпилируем программу с флагом `-O3` и посмотрим на этот же участок листинга

![ror_3](img/ror_O3_1.png)

Видно, что компилятор увидел в программе на С циклический сдвиг и применил замену на более эффективную инструкцию.

Интересно, что подобного рода оптимизации компилятор выполняет даже с ключом `-O1`