from Dataset import CustomDataset




def main():
    filename = "test.owr"
    dataset = CustomDataset(filename)
    indices = [3,4,1,0]
    simples = dataset[indices]
    print(samples)

if __name__ == "__main__":
    main()
    