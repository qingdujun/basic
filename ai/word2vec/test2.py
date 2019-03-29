# -*- coding:utf-8 -*-
import gensim.models as gm

if __name__ == "__main__":
    en_wiki_word2vec_model = gm.Word2Vec.load('wiki.zh.text.model')
    test_words = ['苹果','数学','化妆']
    for word in test_words:
        res = en_wiki_word2vec_model.most_similar(word)
        print(res)