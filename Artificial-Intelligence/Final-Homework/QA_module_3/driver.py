from json import dumps
from QA_module_3 import test_main
from QA_module_3 import test_init
import xlrd
import os


if __name__ == '__main__':
    questions_num = 100
    test_init()
    corrent_num = 0
    data = xlrd.open_workbook('./test.xlsx')
    table = data.sheets()[0]
    if os.path.exists("./output.json"):
        os.remove("./output.json")
    f = open('output.json', 'a+')

    for i in range(0, questions_num, 1):
        tmp_question = table.cell(i, 0).value
        real_ans = table.cell(i, 1).value


        #seg_list = jieba.lcut(u_Question, cut_all=False, HMM=True)
        #answer = Search_All(Question)
        #print(answer)
        ans = test_main(tmp_question)
        # print(ans)
        if ans == real_ans:
            corrent_num+=1

        dic = {'question': tmp_question, 'answer': ans}
        js = dumps(dic, sort_keys=False, indent=4, separators=(',', ':'), ensure_ascii=False)
        print(js, file=f)
    print("corrent number is %d" %corrent_num)
    f.close()
