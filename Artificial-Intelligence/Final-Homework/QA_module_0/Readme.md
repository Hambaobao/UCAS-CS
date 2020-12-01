## 人工智能基础大作业 Module2   
---

文件|说明
-|-
src_train.txt|训练数据(问题)
src_val.txt|验证数据(问题)
tgt_train.txt|训练数据(答案)
tgt_val.txt|验证数据(答案)
--- 

## 使用方法 
1. 下载安装OpenNMT：
```    
    a) pip install OpenNMT-py   
    b) pip install -r requirements.opt.txt  
```
2.  将本文件夹内的4个.txt文件复制到OpenNMT-py的data文件夹下 
3.  预处理数据(执行)：
```
onmt_preprocess -train_src data/src-train.txt -train_tgt data/tgt-train.txt -valid_src data/src-val.txt -valid_tgt data/tgt-val.txt -save_data data/demo
```
4.  训练模型(个人PC耗时较长，大约6小时)：
```
onmt_train -data data/demo -save_model demo-model
```
5.  测试翻译结果：
```
onmt_translate -model demo-model_acc_XX.XX_ppl_XXX.XX_eX.pt -src data/src-test.txt -output pred.txt -replace_unk -verbose
```
---