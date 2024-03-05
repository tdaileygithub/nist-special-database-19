import io
import sqlite3

import numpy as np

import matplotlib.image as mpimg

import torch
from torch.utils.data import Dataset

class Sd19Dataset(Dataset):
    
    _train_test_split_percent = 0.80

    def __init__(self, db_file):
        super().__init__()
        
        self._dataset_ids = {}
        self._total_dataset_length = 0
        self._class_map = {}
        self._con = None
        self._transform = None

        self._con = sqlite3.connect(db_file)
        cur = self._con.cursor()
        cur.execute("SELECT count(*) from mis")
        self._total_dataset_length  = cur.fetchone()[0]
        cur.close()

        cur = self._con.cursor()        
        for row in cur.execute("select character,ROW_NUMBER() OVER(ORDER BY character asc) as classid from mis group by character"):   
            self._class_map[row[0]] = int (row[1]) -1        
        cur.close()

    def __len__(self):
        return len(self._dataset_ids)

    def __getitem__(self, idx):
        if torch.is_tensor(idx):
            idx = idx.tolist()
        
        cur = self._con.cursor()
        cur.execute("SELECT image,character from mis where id=" + str( self._dataset_ids[idx]  ))
        res = cur.fetchone()
        image_blob = res[0]
        image_class = res[1]
        cur.close()
        
        img_bytes = io.BytesIO(image_blob)

        #shape 32x32 
        image = mpimg.imread(img_bytes, format='PNG')
        img_tensor = torch.from_numpy(image)
        
        #one channel, 32x32 size
        img_tensor = img_tensor.view(1, 32, 32)

        return img_tensor, self._class_map[image_class]