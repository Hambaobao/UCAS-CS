import numpy as np
import pandas as pd
import jieba
import re
import string
from string import punctuation
from sklearn.feature_extraction.text import CountVectorizer, TfidfVectorizer
from gensim import similarities
import json

def sentence_to_word(s):
    punctuation = '！？｡ ↓ • 。《 》 ＂→ ＃＄％＆＇（）＊＋，－／：；＜＝＞＠［＼］＾＿｀｛｜｝～｟｠｢｣､、〃》「」『』【】〔〕〖〗〘〙〚〛〜〝〞〟〾〿–—‘’‛“”„‟…‧﹏.'
    zhuci = '了呢与不且之为兮其阿却个以们似若只在来的罢而如呀吗是'
    exclude = list(set(string.punctuation+string.ascii_letters+punctuation+zhuci))
    if type(s)==str:
        s = ''.join(ch for ch in s if ch not in exclude)
        return s
    else: return str()


def cut_word(sentence):
    time_temp = []
    re1 = ".*(\d{4}年)"
    re2 = ".*(\d{1,2}月)"
    re3 = ".*(\d{1,2}日)"
    match1 = re.match(re1, sentence)
    match2 = re.match(re2, sentence)
    match3 = re.match(re3, sentence)
    if match1:
        time = match1.group(1)
        time_temp.append(time)
        sentence = sentence.replace(time,'')
    if match2:
        time = match2.group(1)
        time_temp.append(time)
        sentence = sentence.replace(time,'')
    if match3:
        time = match3.group(1)
        time_temp.append(time)
        sentence = sentence.replace(time,'')
    words = jieba.lcut(sentence, cut_all = False)
    for t in time_temp:
        words.append(t)
    return words


def prepare():
	data1 = pd.read_excel('data1.xlsx')
	data2 = pd.read_excel('data2.xlsx')
	data1.columns = ["question", "answer", 'type', 'a', 'b', 'c', 'd', 'e']
	data2.columns = ["question", "answer", 'type', 'a', 'b', 'c', 'd', 'e']
	data = pd.concat([data1, data2], axis = 0)
	useful_data = data[['question','answer']]
	index = [str(item) for item in list(range(14702))]
	useful_data.index = index
	useful_data = useful_data.dropna(axis = 0, how = 'any')
	return useful_data


def handle_question(question):
	question = sentence_to_word(question)
	question = cut_word(question)
	question = " ".join(question)
	return question


def main():
	jieba.load_userdict("./ai_dic.txt")
	useful_data = prepare()
	questions = useful_data['question'].values
	answers = useful_data["answer"].values
	answers = list(answers)
	questions = [handle_question(question) for question in questions]
	cv = TfidfVectorizer(analyzer = 'word', stop_words = None, max_features = 10000)
	cv.fit(questions)
	questions_vectors = cv.transform(questions)
	num_fea = questions_vectors.shape[1]
	sim_model = similarities.SparseMatrixSimilarity(questions_vectors, num_features=num_fea, num_best = 5)

	while True:
		question = input("please input the question:")
		question_u = handle_question(question)
		vector = cv.transform([question_u])
		index = sim_model[vector]
		output_answer  = {"answer":answers[index[0][0]] , "question": question}
		#json_str = json.dumps(output_answer)
		#print(json_str)
		print("The answer is")
		print(answers[index[0][0]])


global test_cv
global test_sim_model
global test_answers

def test_init():
	global test_cv
	global test_sim_model
	global test_answers
	jieba.load_userdict("./ai_dic.txt")
	# useful_data = prepare()
	# questions = useful_data['question'].values
	# #answers = useful_data["answer"].values
	# #answers = list(answers)
	# test_answers = useful_data["answer"].values
	# test_answers = list(test_answers)
	# questions = [handle_question(question) for question in questions]
	# test_cv = TfidfVectorizer(analyzer = 'word', stop_words = None)
	# test_cv.fit(questions)
	# questions_vectors = test_cv.transform(questions)
	# num_fea = questions_vectors.shape[1]
	# test_sim_model = similarities.SparseMatrixSimilarity(questions_vectors, num_features=num_fea, num_best = 5)

	useful_data = prepare()
	questions = useful_data['question'].values
	test_answers = useful_data["answer"].values
	test_answers = list(test_answers)
	questions = [handle_question(question) for question in questions]
	test_cv = TfidfVectorizer(analyzer = 'word', stop_words = None, max_features = 10000)
	test_cv.fit(questions)
	questions_vectors = test_cv.transform(questions)
	num_fea = questions_vectors.shape[1]
	test_sim_model = similarities.SparseMatrixSimilarity(questions_vectors, num_features=num_fea, num_best = 5)



	# cv = TfidfVectorizer(analyzer = 'word', stop_words = None)
	# cv.fit(questions)
	# questions_vectors = cv.transform(questions)
	# num_fea = questions_vectors.shape[1]
	# sim_model = similarities.SparseMatrixSimilarity(questions_vectors, num_features=num_fea, num_best = 5)
	#



def test_main(question):
	global test_cv
	global test_sim_model
	global test_answers
	# question = input("please input the question:")
	# question = handle_question(question)
	# vector = cv.transform([question])
	# index = sim_model[vector]
	# print("The answer is")
	# print(answers[index[0][0]])
	#question = input("please input the question:")
	# question = handle_question(question)
	# vector = test_cv.transform([question])
	# index = test_sim_model[vector]

	question_u = handle_question(question)
	vector = test_cv.transform([question_u])
	index = test_sim_model[vector]
	output_answer  = {"answer":test_answers[index[0][0]] , "question": question}
	#json_str = json.dumps(output_answer)
	#print(json_str)
	return test_answers[index[0][0]]


	#for i in range(5):
	#	print(answers[index[i][0]])

if __name__ == '__main__':
	main()
