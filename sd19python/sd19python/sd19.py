from multiprocessing import Process, freeze_support

import os

import torch
import torch.optim as optim
import torch.nn.functional as F

from torch.utils.data import DataLoader
from torch.optim.lr_scheduler import StepLR
from torch.optim import lr_scheduler

from Sd19Dataset import Sd19Dataset
from Sd19TrainDataset import Sd19TrainDataset
from Sd19TestDataset import Sd19TestDataset
from Sd19Net import Sd19Net

def train(model, device, train_loader, optimizer, epoch):
    model.train()
    for batch_idx, (data, target) in enumerate(train_loader):
        data = data.to(device)
        target = target.to(device)
        optimizer.zero_grad()
        output = model(data)
        loss = F.nll_loss(output, target)
        loss.backward()
        optimizer.step()
        optimizer.zero_grad() 

        if batch_idx % 50 == 0:
            print('Train Epoch: {} Batch: {} [{}/{} ({:.0f}%)]\tLoss: {:.6f}'.format(
                epoch, batch_idx, batch_idx * len(data), len(train_loader.dataset),
                100. * batch_idx / len(train_loader), loss.item()))

def test(model, device, test_loader):
    model.eval()
    test_loss = 0
    correct = 0
    with torch.no_grad():
        for data, target in test_loader:
            data, target = data.to(device), target.to(device)
            output = model(data)
            test_loss += F.nll_loss(output, target, reduction='sum').item()  # sum up batch loss
            pred = output.argmax(dim=1, keepdim=True)  # get the index of the max log-probability
            correct += pred.eq(target.view_as(pred)).sum().item()

    test_loss /= len(test_loader.dataset)

    print('\nTest set: Average loss: {:.4f}, Accuracy: {}/{} ({:.0f}%)\n'.format(
        test_loss, correct, len(test_loader.dataset),
        100. * correct / len(test_loader.dataset)))

if __name__ == "__main__":
    freeze_support()
    torch.manual_seed(100)
    
    dataset_train = Sd19TrainDataset(db_file="../../out/build/x64-release/sd19.db3")
    dataset_test = Sd19TestDataset(db_file="../../out/build/x64-release/sd19.db3")
    
    train_loader = torch.utils.data.DataLoader(dataset_train,sampler=None, batch_size=750, batch_sampler=None, num_workers=0)
    test_loader = torch.utils.data.DataLoader(dataset_test,sampler=None, batch_size=750, batch_sampler=None, num_workers=0)

    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    print('Using device:', device)
    
    model = Sd19Net().to(device)
    optimizer = optim.Adadelta(model.parameters(), lr=1.0)
    scheduler = StepLR(optimizer, step_size=1, gamma=0.7)

    for epoch in range(1, 10):
        train(model, device, train_loader, optimizer, epoch)
        torch.save(model.state_dict(), "mnist_cnn.pt")
        
        test(model, device, test_loader)
        scheduler.step()