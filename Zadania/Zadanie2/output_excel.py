import xlwt
import statistics

book = xlwt.Workbook(encoding="utf-8")
sheet = book.add_sheet("Sheet 1")

sheet.write(0, 0, "Udaje v tabulkach su uvedene v sekundach")

def excelPriemer():
    x = 0
    for j in range(4):
        if j == 0:
            x = 0
        elif j == 1:
            x = 13
        elif j == 2:
            x = 26
        elif j == 3:
            x = 40

        for i in range(4, 13, 2):
            sheet.write(i, x, "Priemer")

excelPriemer()

sheet.write(1, 0, "Stromy - nahodne cisla")
sheet.write(1, 13, "Stromy - vkladanie na \"striedacku\"")
sheet.write(1, 26, "Stromy - postupnosti")
sheet.write(1, 40, "Hashovanie - nahodne slova")

sheet.write(2, 0, "TEST")
sheet.write(2, 1, "BVS")
sheet.write(2, 5, "AVL")
sheet.write(2, 9, "RB")
sheet.write(2, 13, "TEST")
sheet.write(2, 14, "BVS")
sheet.write(2, 18, "AVL")
sheet.write(2, 22, "RB")
sheet.write(2, 26, "TEST")
sheet.write(2, 27, "BVS")
sheet.write(2, 31, "AVL")
sheet.write(2, 35, "RB")
sheet.write(2, 39, "Pocet vykonanych resize")
sheet.write(2, 40, "TEST")
sheet.write(2, 41, "MOJ HASH")
sheet.write(2, 44, "Pocet vykonanych resize")
sheet.write(2, 45, "CUDZI HASH")


sheet.write(3, 1, "Insert 10")
sheet.write(3, 5, "Insert 10")
sheet.write(3, 9, "Insert 10")
sheet.write(3, 14, "Insert 10")
sheet.write(3, 18, "Insert 10")
sheet.write(3, 22, "Insert 10")
sheet.write(3, 27, "Insert 10")
sheet.write(3, 31, "Insert 10")
sheet.write(3, 35, "Insert 10")
sheet.write(3, 41, "Insert 10")
sheet.write(3, 45, "Insert 10")
sheet.write(3, 3, "Search")
sheet.write(3, 7, "Search")
sheet.write(3, 11, "Search")
sheet.write(3, 16, "Search")
sheet.write(3, 20, "Search")
sheet.write(3, 24, "Search")
sheet.write(3, 29, "Search")
sheet.write(3, 33, "Search")
sheet.write(3, 37, "Search")
sheet.write(3, 43, "Search")
sheet.write(3, 47, "Search")

sheet.write(5, 1, "Insert 100")
sheet.write(5, 5, "Insert 100")
sheet.write(5, 9, "Insert 100")
sheet.write(5, 14, "Insert 100")
sheet.write(5, 18, "Insert 100")
sheet.write(5, 22, "Insert 100")
sheet.write(5, 27, "Insert 100")
sheet.write(5, 31, "Insert 100")
sheet.write(5, 35, "Insert 100")
sheet.write(5, 41, "Insert 100")
sheet.write(5, 45, "Insert 100")
sheet.write(5, 3, "Search")
sheet.write(5, 7, "Search")
sheet.write(5, 11, "Search")
sheet.write(5, 16, "Search")
sheet.write(5, 20, "Search")
sheet.write(5, 24, "Search")
sheet.write(5, 29, "Search")
sheet.write(5, 33, "Search")
sheet.write(5, 37, "Search")
sheet.write(5, 43, "Search")
sheet.write(5, 47, "Search")

sheet.write(7, 1, "Insert 1 000")
sheet.write(7, 5, "Insert 1 000")
sheet.write(7, 9, "Insert 1 000")
sheet.write(7, 14, "Insert 1 000")
sheet.write(7, 18, "Insert 1 000")
sheet.write(7, 22, "Insert 1 000")
sheet.write(7, 27, "Insert 1 000")
sheet.write(7, 31, "Insert 1 000")
sheet.write(7, 35, "Insert 1 000")
sheet.write(7, 41, "Insert 1 000")
sheet.write(7, 45, "Insert 1 000")
sheet.write(7, 3, "Search")
sheet.write(7, 7, "Search")
sheet.write(7, 11, "Search")
sheet.write(7, 16, "Search")
sheet.write(7, 20, "Search")
sheet.write(7, 24, "Search")
sheet.write(7, 29, "Search")
sheet.write(7, 33, "Search")
sheet.write(7, 37, "Search")
sheet.write(7, 43, "Search")
sheet.write(7, 47, "Search")

sheet.write(9, 1, "Insert 10 000")
sheet.write(9, 5, "Insert 10 000")
sheet.write(9, 9, "Insert 10 000")
sheet.write(9, 14, "Insert 10 000")
sheet.write(9, 18, "Insert 10 000")
sheet.write(9, 22, "Insert 10 000")
sheet.write(9, 27, "Insert 10 000")
sheet.write(9, 31, "Insert 10 000")
sheet.write(9, 35, "Insert 10 000")
sheet.write(9, 41, "Insert 10 000")
sheet.write(9, 45, "Insert 10 000")
sheet.write(9, 3, "Search")
sheet.write(9, 7, "Search")
sheet.write(9, 11, "Search")
sheet.write(9, 16, "Search")
sheet.write(9, 20, "Search")
sheet.write(9, 24, "Search")
sheet.write(9, 29, "Search")
sheet.write(9, 33, "Search")
sheet.write(9, 37, "Search")
sheet.write(9, 43, "Search")
sheet.write(9, 47, "Search")

sheet.write(11, 1, "Insert 100 000")
sheet.write(11, 5, "Insert 100 000")
sheet.write(11, 9, "Insert 100 000")
sheet.write(11, 14, "Insert 100 000")
sheet.write(11, 18, "Insert 100 000")
sheet.write(11, 22, "Insert 100 000")
sheet.write(11, 27, "Insert 100 000")
sheet.write(11, 31, "Insert 100 000")
sheet.write(11, 35, "Insert 100 000")
sheet.write(11, 41, "Insert 100 000")
sheet.write(11, 45, "Insert 100 000")
sheet.write(11, 3, "Search")
sheet.write(11, 7, "Search")
sheet.write(11, 11, "Search")
sheet.write(11, 16, "Search")
sheet.write(11, 20, "Search")
sheet.write(11, 24, "Search")
sheet.write(11, 29, "Search")
sheet.write(11, 33, "Search")
sheet.write(11, 37, "Search")
sheet.write(11, 43, "Search")
sheet.write(11, 47, "Search")

txtf = open(r'C:\Users\emari\projects\DSA\Zadania\Zadanie2\output.txt', 'r')
algorithm = ""

myHashIns10 = []
myHashIns100 = []
myHashIns1000 = []
myHashIns10000 = []
myHashIns100000 = []
notHashIns10 = []
notHashIns100 = []
notHashIns1000 = []
notHashIns10000 = []
notHashIns100000 = []
myHashSea10 = []
myHashSea100 = []
myHashSea1000 = []
myHashSea10000 = []
myHashSea100000 = []
notHashSea10 = []
notHashSea100 = []
notHashSea1000 = []
notHashSea10000 = []
notHashSea100000 = []

bvsIns10 = []
bvsIns100 = []
bvsIns1000 = []
bvsIns10000 = []
bvsIns100000 = []
avlIns10 = []
avlIns100 = []
avlIns1000 = []
avlIns10000 = []
avlIns100000 = []
rbIns10 = []
rbIns100 = []
rbIns1000 = []
rbIns10000 = []
rbIns100000 = []
bvsSea10 = []
bvsSea100 = []
bvsSea1000 = []
bvsSea10000 = []
bvsSea100000 = []
avlSea10 = []
avlSea100 = []
avlSea1000 = []
avlSea10000 = []
avlSea100000 = []
rbSea10 = []
rbSea100 = []
rbSea1000 = []
rbSea10000 = []
rbSea100000 = []
global floatList
floatList = []

def hashExcel(cutLine):
    if str(cutLine[0]) == "insert":
        if "my" in str(cutLine[1]):
            if str(cutLine[1]).endswith("10"):
                myHashIns10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                myHashIns100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                myHashIns1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                myHashIns10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                myHashIns100000.append(str(cutLine[2]))

        elif "not" in str(cutLine[1]):
            if str(cutLine[1]).endswith("10"):
                notHashIns10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                notHashIns100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                notHashIns1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                notHashIns10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                notHashIns100000.append(str(cutLine[2]))

    elif str(cutLine[0]) == "search":
        if "my" in str(cutLine[1]):
            if str(cutLine[1]).endswith("10"):
                myHashSea10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                myHashSea100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                myHashSea1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                myHashSea10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                myHashSea100000.append(str(cutLine[2]))

        elif "not" in str(cutLine[1]):
            if str(cutLine[1]).endswith("10"):
                notHashSea10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                notHashSea100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                notHashSea1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                notHashSea10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                notHashSea100000.append(str(cutLine[2]))

    else:
        pass


def treeExcel(cutLine):
    if str(cutLine[0]) == "insert":
        if str(cutLine[1]).startswith("bvs"):
            if str(cutLine[1]).endswith("10"):
                bvsIns10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                bvsIns100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                bvsIns1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                bvsIns10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                bvsIns100000.append(str(cutLine[2]))

        elif str(cutLine[1]).startswith("avl"):
            if str(cutLine[1]).endswith("10"):
                avlIns10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                avlIns100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                avlIns1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                avlIns10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                avlIns100000.append(str(cutLine[2]))

        elif str(cutLine[1]).startswith("rb"):
            if str(cutLine[1]).endswith("10"):
                rbIns10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                rbIns100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                rbIns1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                rbIns10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                rbIns100000.append(str(cutLine[2]))

    elif str(cutLine[0]) == "search":
        if str(cutLine[1]).startswith("bvs"):
            if str(cutLine[1]).endswith("10"):
                bvsSea10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                bvsSea100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                bvsSea1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                bvsSea10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                bvsSea100000.append(str(cutLine[2]))

        elif str(cutLine[1]).startswith("avl"):
            if str(cutLine[1]).endswith("10"):
                avlSea10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                avlSea100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                avlSea1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                avlSea10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                avlSea100000.append(str(cutLine[2]))

        elif str(cutLine[1]).startswith("rb"):
            if str(cutLine[1]).endswith("10"):
                rbSea10.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100"):
                rbSea100.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("1000"):
                rbSea1000.append(str(cutLine[2]))            
            elif str(cutLine[1]).endswith("10000"):
                rbSea10000.append(str(cutLine[2]))
            elif str(cutLine[1]).endswith("100000"):
                rbSea100000.append(str(cutLine[2]))

    else:
        pass


count = 0

for line in txtf:
    if not line or line == "\n":
        if (algorithm == "hash"):
            # ------------------------------------------------My hash insert
            for num in myHashIns10:
                floatList.append(float(num))
            
            sheet.write(4, 41, str(statistics.mean(floatList)))
            floatList.clear()

            for num in myHashIns100:
                floatList.append(float(num))
            
            sheet.write(6, 41, str(statistics.mean(floatList)))
            floatList.clear()

            for num in myHashIns1000:
                floatList.append(float(num))
            
            sheet.write(8, 41, str(statistics.mean(floatList)))
            floatList.clear()

            for num in myHashIns10000:
                floatList.append(float(num))
            
            sheet.write(10, 41, str(statistics.mean(floatList)))
            floatList.clear()

            for num in myHashIns100000:
                floatList.append(float(num))
            
            sheet.write(12, 41, str(statistics.mean(floatList)))
            floatList.clear()
            # ------------------------------------------------My hash search
            for num in myHashSea10:
                floatList.append(float(num))
            
            sheet.write(4, 43, str(statistics.mean(floatList)))
            floatList.clear()

            for num in myHashSea100:
                floatList.append(float(num))
            
            sheet.write(6, 43, str(statistics.mean(floatList)))
            floatList.clear()

            for num in myHashSea1000:
                floatList.append(float(num))
            
            sheet.write(8, 43, str(statistics.mean(floatList)))
            floatList.clear()

            for num in myHashSea10000:
                floatList.append(float(num))
            
            sheet.write(10, 43, str(statistics.mean(floatList)))
            floatList.clear()

            for num in myHashSea100000:
                floatList.append(float(num))
            
            sheet.write(12, 43, str(statistics.mean(floatList)))
            floatList.clear()
            # ------------------------------------------------Not hash insert
            for num in notHashIns10:
                floatList.append(float(num))
            
            sheet.write(4, 45, str(statistics.mean(floatList)))
            floatList.clear()

            for num in notHashIns100:
                floatList.append(float(num))
            
            sheet.write(6, 45, str(statistics.mean(floatList)))
            floatList.clear()

            for num in notHashIns1000:
                floatList.append(float(num))
            
            sheet.write(8, 45, str(statistics.mean(floatList)))
            floatList.clear()

            for num in notHashIns10000:
                floatList.append(float(num))
            
            sheet.write(10, 45, str(statistics.mean(floatList)))
            floatList.clear()

            for num in notHashIns100000:
                floatList.append(float(num))
            
            sheet.write(12, 45, str(statistics.mean(floatList)))
            floatList.clear()
            # ------------------------------------------------Not hash search
            for num in notHashSea10:
                floatList.append(float(num))
            
            sheet.write(4, 47, str(statistics.mean(floatList)))
            floatList.clear()

            for num in notHashSea100:
                floatList.append(float(num))
            
            sheet.write(6, 47, str(statistics.mean(floatList)))
            floatList.clear()

            for num in notHashSea1000:
                floatList.append(float(num))
            
            sheet.write(8, 47, str(statistics.mean(floatList)))
            floatList.clear()

            for num in notHashSea10000:
                floatList.append(float(num))
            
            sheet.write(10, 47, str(statistics.mean(floatList)))
            floatList.clear()

            for num in notHashSea100000:
                floatList.append(float(num))
            
            sheet.write(12, 47, str(statistics.mean(floatList)))
            floatList.clear()
            

        if (algorithm == "tree"):
            y = 0
            
            if count == 0:
                y = 1
            elif count == 1:
                y = 14
            elif count == 2:
                y = 27

            count += 1
            
            # ------------------------------------------------bvs insert
            for num in bvsIns10:
                floatList.append(float(num))
            
            sheet.write(4, y, str(statistics.mean(floatList)))
            floatList.clear()

            for num in bvsIns100:
                floatList.append(float(num))
            
            sheet.write(6, y, str(statistics.mean(floatList)))
            floatList.clear()

            for num in bvsIns1000:
                floatList.append(float(num))
            
            sheet.write(8, y, str(statistics.mean(floatList)))
            floatList.clear()

            for num in bvsIns10000:
                floatList.append(float(num))
            
            sheet.write(10, y, str(statistics.mean(floatList)))
            floatList.clear()

            for num in bvsIns100000:
                floatList.append(float(num))
            
            sheet.write(12, y, str(statistics.mean(floatList)))
            floatList.clear()
            # ------------------------------------------------bvs search
            for num in bvsSea10:
                floatList.append(float(num))
            
            sheet.write(4, y+2, str(statistics.mean(floatList)))
            floatList.clear()

            for num in bvsSea100:
                floatList.append(float(num))
            
            sheet.write(6, y+2, str(statistics.mean(floatList)))
            floatList.clear()

            for num in bvsSea1000:
                floatList.append(float(num))
            
            sheet.write(8, y+2, str(statistics.mean(floatList)))
            floatList.clear()

            for num in bvsSea10000:
                floatList.append(float(num))
            
            sheet.write(10, y+2, str(statistics.mean(floatList)))
            floatList.clear()

            for num in bvsSea100000:
                floatList.append(float(num))
            
            sheet.write(12, y+2, str(statistics.mean(floatList)))
            floatList.clear()
            # ------------------------------------------------avl insert
            for num in avlIns10:
                floatList.append(float(num))
            
            sheet.write(4, y+4, str(statistics.mean(floatList)))
            floatList.clear()

            for num in avlIns100:
                floatList.append(float(num))
            
            sheet.write(6, y+4, str(statistics.mean(floatList)))
            floatList.clear()

            for num in avlIns1000:
                floatList.append(float(num))
            
            sheet.write(8, y+4, str(statistics.mean(floatList)))
            floatList.clear()

            for num in avlIns10000:
                floatList.append(float(num))
            
            sheet.write(10, y+4, str(statistics.mean(floatList)))
            floatList.clear()

            for num in avlIns100000:
                floatList.append(float(num))
            
            sheet.write(12, y+4, str(statistics.mean(floatList)))
            floatList.clear()
            # ------------------------------------------------avl search
            for num in avlSea10:
                floatList.append(float(num))
            
            sheet.write(4, y+6, str(statistics.mean(floatList)))
            floatList.clear()

            for num in avlSea100:
                floatList.append(float(num))
            
            sheet.write(6, y+6, str(statistics.mean(floatList)))
            floatList.clear()

            for num in avlSea1000:
                floatList.append(float(num))
            
            sheet.write(8, y+6, str(statistics.mean(floatList)))
            floatList.clear()

            for num in avlSea10000:
                floatList.append(float(num))
            
            sheet.write(10, y+6, str(statistics.mean(floatList)))
            floatList.clear()

            for num in avlSea100000:
                floatList.append(float(num))
            
            sheet.write(12, y+6, str(statistics.mean(floatList)))
            floatList.clear()
            # ------------------------------------------------rb insert
            for num in rbIns10:
                floatList.append(float(num))
            
            sheet.write(4, y+8, str(statistics.mean(floatList)))
            floatList.clear()

            for num in rbIns100:
                floatList.append(float(num))
            
            sheet.write(6, y+8, str(statistics.mean(floatList)))
            floatList.clear()

            for num in rbIns1000:
                floatList.append(float(num))
            
            sheet.write(8, y+8, str(statistics.mean(floatList)))
            floatList.clear()

            for num in rbIns10000:
                floatList.append(float(num))
            
            sheet.write(10, y+8, str(statistics.mean(floatList)))
            floatList.clear()

            for num in rbIns100000:
                floatList.append(float(num))
            
            sheet.write(12, y+8, str(statistics.mean(floatList)))
            floatList.clear()
            # ------------------------------------------------rb search
            for num in rbSea10:
                floatList.append(float(num))
            
            sheet.write(4, y+10, str(statistics.mean(floatList)))
            floatList.clear()

            for num in rbSea100:
                floatList.append(float(num))
            
            sheet.write(6, y+10, str(statistics.mean(floatList)))
            floatList.clear()

            for num in rbSea1000:
                floatList.append(float(num))
            
            sheet.write(8, y+10, str(statistics.mean(floatList)))
            floatList.clear()

            for num in rbSea10000:
                floatList.append(float(num))
            
            sheet.write(10, y+10, str(statistics.mean(floatList)))
            floatList.clear()

            for num in rbSea100000:
                floatList.append(float(num))
            
            sheet.write(12, y+10, str(statistics.mean(floatList)))
            floatList.clear()
    else:
        cutLine = line.split()

        if "HASHOVANIE-" == str(cutLine[0]):
            algorithm = "hash"

        elif "STROMY-" == str(cutLine[0]):
            algorithm = "tree"

        if (algorithm == "hash"):
            hashExcel(cutLine)

        if (algorithm == "tree"):
            treeExcel(cutLine)

        
    
        



book.save("testovanie2.xls")
txtf.close()