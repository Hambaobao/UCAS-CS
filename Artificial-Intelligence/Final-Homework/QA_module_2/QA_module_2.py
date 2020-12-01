import sys
import jieba
import difflib

line_numb = 0
DataSets = []


QUESTION_INDEX = 0
ANSWER_INDEX = 1
QA_TYPE = 2
DOMAIN = 3
SEMANTIC = 4

'''
WHETHER = 'Whether'
WHERE = 'Where'
WHICH = 'Which'
WHEN = 'When'
WHAT = 'What'
WHO = 'Who'
WHY = 'Why'
HOW = 'How'


QA_Whether = []
QA_Where = []
QA_Which = []
QA_When = []
QA_What = []
QA_Who = []
QA_Why = []
QA_How = []
'''

def ReadTxtName(rootdir):
    global line_numb
    line_numb = 0
    with open(rootdir, 'r') as file_to_read:
        while True:
            line = file_to_read.readline()
            if not line:
                break
            line = line.strip().split('\t')
            DataSets.append(line)
            line_numb = line_numb + 1
    return DataSets

def pre_data(rootdir):
    line_numb0 = len(DataSets)
    ReadTxtName(rootdir)
'''    
#    print(line_numb)
    for i in range(line_numb0, line_numb0 + line_numb):
        if DataSets[i][QA_TYPE] == WHETHER:
#            print(DataSets[i])
            QA_Whether.append(DataSets[i])
        elif DataSets[i][QA_TYPE] == WHERE:
#            print(DataSets[i])
            QA_Where.append(DataSets[i])
        elif DataSets[i][QA_TYPE] == WHICH:
#            print(DataSets[i])
            QA_Which.append(DataSets[i])
        elif DataSets[i][QA_TYPE] == WHEN:
#            print(DataSets[i])
            QA_When.append(DataSets[i])
        elif DataSets[i][QA_TYPE] == WHAT:
#            print(DataSets[i])
            QA_What.append(DataSets[i])
        elif DataSets[i][QA_TYPE] == WHO:
#            print(DataSets[i])
            QA_Who.append(DataSets[i])            
        elif DataSets[i][QA_TYPE] == WHY:
#            print(DataSets[i])
            QA_Why.append(DataSets[i])
        elif DataSets[i][QA_TYPE] == HOW:
#            print(DataSets[i])
            QA_How.append(DataSets[i])
        else:
            print(i)
            print(DataSets[i])
            break
'''        
'''
    print("QA_Whether Count = %d" % len(QA_Whether))
    print("QA_Where Count = %d" % len(QA_Where))
    print("QA_Which Count = %d" % len(QA_Which))
    print("QA_When Count = %d" % len(QA_When))
    print("QA_Who Count = %d" % len(QA_Who))
    print("QA_Why Count = %d" % len(QA_Why))
    print("QA_How Count = %d" % len(QA_How))
    QA_Sum = len(QA_Whether) + len(QA_Where) + len(QA_Which) + len(QA_When) + len(QA_What)+ len(QA_Who) + len(QA_Why) + len(QA_How)
    print("QA_Sum = %d" % QA_Sum)
'''

def string_similar(s1, s2):
    return difflib.SequenceMatcher(None, s1, s2).quick_ratio()

'''
def Search_What(question):
    similar_list = []
    print(len(QA_What))
    for i in range(len(QA_What)):
        similar_list.append(string_similar(question, QA_What[i][QUESTION_INDEX]))
        print(QA_What[i][QUESTION_INDEX]) 
    print(similar_list)       
'''

def Search_All(question):
    similar_list = []
    for i in range(len(DataSets)):
        similar_list.append(string_similar(question, DataSets[i][QUESTION_INDEX]))
    return DataSets[similar_list.index(max(similar_list))][ANSWER_INDEX] 

if __name__ == "__main__":
    pre_data('data/data1.txt')
    pre_data('data/data2.txt')

    while True:
        Question = input("请输入问题:")
        u_Question = Question.encode("UTF-8")
        seg_list = jieba.lcut(u_Question, cut_all=False, HMM=True)
        answer = Search_All(Question)
        print(answer)

