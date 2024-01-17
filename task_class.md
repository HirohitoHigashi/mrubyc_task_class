# mruby/c のスケジューラに追加する Task class の検討用

## 名前

クラス名 `Task`

検討の余地あり。
* prefix をつけるか
* 全く別の名前 （Job とか。ミニコン/汎用機用語かも？）


## 機能（やりたいこと）

1. タスクリストの保持
2. タスクに名前をつけて識別
3. タスクオブジェクトの生成
4. 一時停止
5. 再開
6. 終了
7. 他タスクの終了待ち
8. タスクプライオリティーの変更
9. 実行権を手放す
10. 状態の取得


## 1. タスクリストの保持

Task.list -> Array[Task]  
Task.name_list -> Array["Name"]


## 2. タスクに名前をつけて識別

```
Task.name = "Task1"
```

```
def name=: (String) -> String
def name -> String
```

（検討）setterの戻り値は、void でも良いか？


## 3. タスクオブジェクトの生成

```
task1 = Task.get             # 自タスク
task2 = Task.get("TaskName") # その名前を持つタスク
```
指定のタスクが存在しない場合は、nil が返る。

※ 指定のタスク実行完了待ちは、こう？
```
until task1 = Task.get("Task1")
  sleep 1
end
task1.join
```


※ 一旦 new は取り下げ。  
（検討）
Task.new だと、新しいタスクを作るみたい。実際は今動いているタスクを操作するためのオブジェクトを得るという操作。
Task.get とかの方が良いか？

```
task1 = Task.new             # 自タスク
task2 = Task.new("TaskName") # その名前を持つタスク
```
指定の名前を持ったタスクが存在しない場合、`ArgumentError`



## 4. 一時停止

自タスクの停止

```
Task.suspend
```

他タスクを停止

```
task1.suspend
```

## 5. 再開

```
task1.resume
```

## 6. 終了

自タスクの終了

```
Task.terminate
```

他タスクの終了

```
task1.terminate
```

## 7. 他タスクの終了待ち

  def join: (?Integer|nil) -> (self | nil)

```
task1.join
```

（検討）
Threadクラスには、引数としてタイムアウト秒を渡せるが、実装が面倒かも？


## 8. タスクプライオリティーの変更
```
task.priority = n    # 0(優先度高) - 255(優先度低)
```

## 9. 実行権を手放す
```
Task.pass
```

## 10. 状態の取得

```
task.status
```

戻り値  
スケジューラの状態を文字列で返せば良いか？

* "DORMANT"
* "READY"
* "WAITING"
* "SUSPENDED"

WAITINGは、"SLEEP" などサブステータスがある。"WAITING SLEEP" とかの方が良いか？
