import pickle
from ownrecord import FileReader


class CustomDataset(FileReader):

    def __getitem__(self, *index):
        result = []
        idx = [it for it in index[0]]
        byte_ = super().read(idx)
        for data in byte_:
            result.append(pickle.loads(data))
        return result
