import sqlite3

from Sd19Dataset import Sd19Dataset

class Sd19TrainDataset(Sd19Dataset):
  def __init__(self, db_file):
    super().__init__(db_file)
    
    last_id = int(self._train_test_split_percent * self._total_dataset_length)

    cur = self._con.cursor()
    idx = 0
    for row in cur.execute("SELECT id from mis where id <" + str( last_id  )):   
        self._dataset_ids[idx] = int (row[0])
        idx += 1
    cur.close()