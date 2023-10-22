// 我的解法
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

// 官方双指针
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
