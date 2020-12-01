import pandas as pd
import jieba

char_to_i = {'金牌': 1, '银牌' : 2, '铜牌' : 3, '排名' : 4}


stop_list = [line.strip() for line in open('stopwordsHIT.txt', encoding='utf-8').readlines()]
def get_critical_data():
    attr = ['国家', '年份', '金牌数', '银牌数', '铜牌数', '排名']
    df = pd.DataFrame(columns = attr)
    all_data = pd.read_csv('标注数据1.csv')
    need = all_data.tail(7220 - 5169)[:-1]
    answer_list = need['answer'].to_list()
    need_questions = need['问答对'].to_list()
    #print(need_questions)
    country_list = [line.strip() for line in open('country.txt', encoding='utf-8').readlines()]
    ques_index = 0
    country_index = 0
    last_country = None
    one_dic = {}
    all_dict = []
    for sent in need_questions:
        split_word = ' '.join(jieba.cut(sent)).split(' ')
        fi_split_word = []
        for word in split_word: 
            if word not in stop_list:
                fi_split_word.append(word)
        #print(fi_split_word)
        flag = 0
        answer = answer_list[ques_index]
        for word in fi_split_word:
            if word.isdigit() and len(word) >= 4:
                flag = 5
                year = word
                #one_dic['year'] = word
            if word == '金牌':
                flag = 1
                #one_dic['gold'] = answer
            elif word == '银牌':
                flag = 2
                #one_dic['silver'] = answer
            elif word == '铜牌':
                flag = 3
                #one_dic['Cu'] = answer
            elif word == '第几名':
                flag = 4
                #one_dic['rank'] = answer
            if word in country_list:
                country = word
        #print(one_dic)
        if last_country != country:
            # add this dic to df:
            if country_index >= 1:
                #print(one_dic)
                fi_ans = []
                fi_ans.append(one_dic['country'])
                fi_ans.append(one_dic['year'])
                fi_ans.append(one_dic['gold'])
                fi_ans.append(one_dic['silver'])
                fi_ans.append(one_dic['cu'])
                fi_ans.append(one_dic['rank'])
                #print(fi_ans)
                temp = pd.DataFrame(fi_ans)
                df.loc[country_index] = fi_ans
                all_dict.append(one_dic)
            country_index += 1
            last_country = country
            one_dic = {}
            one_dic['country'] = country
        if flag == 5:
            one_dic['year'] = year
        elif flag == 1:
            one_dic['gold'] = answer
        elif flag == 2:
            one_dic['silver'] = answer
        elif flag == 3:
            one_dic['cu'] = answer
        elif flag == 4:
            one_dic['rank'] = answer
        ques_index += 1
    return  all_dict


class  CountryNodes:
    def __init__(self, country = None, year = None, gold = None, silver = None, cu = None, rank = None):
        self.country = country
        self.year = year
        self.gold = gold
        self.silver = silver
        self.cu = cu
        self.rank = rank
    
    def get_specific_info(self,flag):
        
        if flag == 1:
            return self.gold

        elif flag == 2:
            return self.silver
        elif flag == 3:
            return self.cu
        elif flag == 4:
            return self.rank

def cmp(str1, str2):
    if len(str1) == len(str2):
        return str1 >= str2
    return len(str1) > len(str2)

def qsort(tmp):
    if len(tmp) == 0:
        return None
    pivort = tmp[0]
    #print(pivort)
    tmp.pop(0)
    lhand = [a for a in tmp if cmp(a['eval'], pivort['eval']) ]
    rhand = [a for a in tmp if cmp(a['eval'] , pivort['eval']) == False]
    #print("lhand is : ", end=" ")
    #print(lhand)
    #print("rhand is : ", end = " ")
    #print(rhand)
    
    lret = qsort(lhand)
    rret = qsort(rhand)
    ret = []
    
    if lret is not None:
        ret = ret + lret
    #ret = ret.append(pivort)
    #print("after append:")
    #print(ret)
    #print("before append:")
    #print(pivort)
    ret.append(pivort)
    #print("after append:")
    #print(ret)
    #ret = ret + list(pivort)
    if rret is not None:
        ret = ret + rret;
    
    return ret

class OlympicNodes:
    '''冬奥会节点'''
    def __init__(self, year):
        self.countries = []
        self.year = year

    def get_info(self, country):
        
        for country in self.countries:
            if country.country == country:
                  return  country
        return None

    def get_top_countries(self, flag, n):
        if n > len(self.countries):
            return None
        
        tmp = self.countries
        #print([country.country for country in tmp])
        tmp_eval = [{'eval': country.get_specific_info(flag), 'name':country.country} for country in tmp]
        #print(tmp_eval)
        tmp = qsort(tmp_eval)
        #print(tmp)
        #print(type(tmp[0]))
        return  [tmp[i]['name'] for i in range(n)]

def build_graph():
    
    all_dict = get_critical_data()
    all_year = []
    for each in all_dict:
        if each['year'] not in all_year:
            all_year.append(each['year'])
    
    all_olympics = {}
    for year in all_year:
        all_olympics[year] = OlympicNodes(year)
    
    for country in all_dict:
        year = country['year']
        all_olympics[year].countries.append(CountryNodes(country['country'], year, country['gold'], country['silver'],
                                            country['cu'], country['rank']))
    return all_olympics

def judge_question(question):
    
    split_ques = ' '.join(jieba.cut(question)).split(' ')
    year = None
    flag = 0
    par = 0
    ques_type = 0
    for word in split_ques:
        if word.isdigit() and len(word) >= 4:
            year = word
        if word.isdigit() and len(word) <= 2:
            par = word
        elif '金牌' in word:
            flag = 1
        elif '银牌' in word:
            flag = 2
        elif '铜牌' in word:
            flag = 3
        elif '排名' in word or '排行' in word:
           if flag != 1 and flag != 2 and flag != 3:
               flag = 4 
    return [year, flag, par]

if __name__ == '__main__':
    all_olympics = build_graph()
    #print(all_olympics['1924'].countries[0].country)
    #print(all_olympics['2018'].countries[0].country)
    #print(all_olympics['2018'].countries[0].cu)
    #a = all_olympics['2018'].get_top_countries(3, 10)
    #print(a)
    
    while True:
        question = input("请输入你的问题:")
        info = judge_question(question)
        #print(info)
        tmp = all_olympics[info[0]].get_top_countries(int(info[1]), int(info[2]))
        #print(tmp)
        if tmp is None:
            print("没有答案")
        
        elif tmp is not None:
            for i in tmp:
                print(i, end=",")
            print("")
            print("")