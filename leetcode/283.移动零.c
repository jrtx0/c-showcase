// 我的解法：遇到零就整体向前移动，效率比较低，可以利用双指针等所有的零都跨过了再移动。
void moveZeroes(int* nums, int numsSize)
{
    int zeroCnt = 0;

    for (int i = 0; i < numsSize; i++)
    {
        if (nums[i] == 0)
        {
            zeroCnt++;
        }
    }

    if (zeroCnt == numsSize)
    {
        return;
    }

    for (int i = 0, k = zeroCnt, t = numsSize - 1; i < numsSize; i++)
    {
        // 遇到 0 就整体向前移动一次直到没有 0
        while (nums[i] == 0)
        {
            if (k-- != 0)
            {
                for (int j = i; j < numsSize - 1; j++)
                {
                    nums[j] = nums[j + 1];
                }
                nums[t--] = 0;
            }
            else
            {
                return;
            }
        }
    }
}

// 我的解法优化：双指针移动直到遇到非零再进行交换
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b, *b = t;
}

void moveZeroes(int* nums, int numsSize){
    int left = 0, right = 0;

    while (right < numsSize)
    {
        if (nums[left] != 0 && nums[right] != 0)
        {
            left++;
            right++;
        }
        else if (nums[left] == 0 && nums[right] == 0)
        {
            right++;
        }
        else if (nums[left] == 0 && nums[right] != 0)
        {
            swap(nums + left, nums + right);
        }
        else
        {
            left++;
        }
    }
}


// 双指针解法：左指针指向当前已经处理好的序列的尾部，右指针指向待处理序列的头部。
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b, *b = t;
}

void moveZeroes(int* nums, int numsSize){
    int left = 0, right = 0;

    while (right < numsSize)
    {
        if (nums[right])
        {
            swap(nums + left, nums + right);
            left++;
        }
        right++;
    }
}
