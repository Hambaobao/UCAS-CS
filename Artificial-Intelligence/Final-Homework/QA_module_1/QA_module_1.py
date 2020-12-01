
from gensim.models.keyedvectors import KeyedVectors
w2v = KeyedVectors.load_word2vec_format('sgns.renmin.bigram-char', binary=False, unicode_errors='ignore')


import pandas as pd
import jieba
import numpy as np

all_data = pd.read_csv('标注数据1.csv', encoding='utf-8')
stop_list = [line.strip() for line in open('stopwordsHIT.txt', encoding='utf-8').readlines()]

questions = all_data['问答对'].tolist()

'''
from gensim.models.keyedvectors import KeyedVectors
w2v = KeyedVectors.load_word2vec_format('sgns.renmin.bigram-char', binary=False, unicode_errors='ignore')
'''
split_ques = []

for sent in questions:
    if type(sent) == str:
        split_ques.append(' '.join(jieba.cut(sent, cut_all=True, HMM=True)).split(' '))

emb_ques = []
for split_sent in split_ques:
    emb_sent = []
    for word in split_sent:
        if word not in w2v:
            emb_sent.append(np.zeros(300))
        else:
            if word not in stop_list:
                emb_sent.append(w2v[word])
    a = np.sum(emb_sent, axis=0)
    emb_ques.append(a)

def cosin_sim(x, y):
    res = float(0)
    for i in range(len(x)):
        res += x[i] * y[i]
    x_square = [a * a for a in x]
    y_square = [a * a for a in y]
    x_v, y_v = (0, 0)
    for i in range(len(x_square)):
        x_v += x_square[i]
        y_v += y_square[i]
    
    return res / (np.sqrt(x_v) * np.sqrt(y_v))

def cut_ques(ques):
    return ' '.join(jieba.cut(ques, cut_all=True, HMM=True)).split(' ')

def get_emb_ques(ques_list):
    ret_ques = []
    for word in ques_list:
        if word not in w2v:
            ret_ques.append(np.zeros(300))
        else:
            if word not in stop_list:
                ret_ques.append(w2v[word])
    return np.sum(ret_ques, axis=0)

def get_ans(embed_ques, data_len):
    cos_list = []
    max_index = 0
    max_val   = 0
    for i in range(data_len):
        a = cosin_sim(embed_ques, emb_ques[i])
        #print(a)
        #print(a)
        #print(max_val)
        if a > max_val:
            max_index = i
            max_val = a
    return (max_index, max_val)


answer_list = all_data['answer'].tolist()

input_question = input("请输入你的问题：")
ques_list = cut_ques(input_question)
embed_ques = get_emb_ques(ques_list)
#print(embed_ques)
max_index, max_val = get_ans(embed_ques, len(answer_list)-1)
#print("max index:%d"%(max_index))
print("max index : %d, max val : %d"%(max_index, max_val))
print("答案：", end=" ")
print(answer_list[max_index])