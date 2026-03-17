import http from '@ohos.net.http';
import promptAction from '@ohos.promptAction';

const BASE_URL: string = "http://172.20.10.3:9000/env/findPageAll";

@Entry
@Component
struct Index {
  @State temp: string = "--";
  @State humi: string = "--";
  @State gas: string = "--";
  @State isLoading: boolean = false;

  aboutToAppear(): void {
    this.loadData();
  }

  build(): void {
    Column() {
      Text("环境监测系统")
        .fontSize(30)
        .fontWeight(FontWeight.Bold)
        .margin(30);

      Column() {
        Text(`温度: ${this.temp} ℃`).fontSize(24).margin(10);
        Text(`湿度: ${this.humi} %`).fontSize(24).margin(10);
        Text(`燃气: ${this.gas}`).fontSize(24);
      }
      .padding(25)
      .backgroundColor('#F5F5F5')
      .borderRadius(15)
      .width('90%');

      Button(this.isLoading ? "加载中..." : "刷新数据")
        .width('90%')
        .height(50)
        .backgroundColor('#009FFB')
        .enabled(!this.isLoading)
        .onClick(() => this.loadData())
        .margin({ top: 30 });
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#fff');
  }

  loadData(): void {
    this.isLoading = true;
    const httpRequest: http.HttpRequest = http.createHttp();

    httpRequest.request(BASE_URL, {
      method: http.RequestMethod.GET,
      connectTimeout: 10000,
      readTimeout: 10000
    }, (err: Error | null, data: http.HttpResponse): void => {
      this.isLoading = false;

      if (err) {
        console.error("请求错误", JSON.stringify(err));
        promptAction.showToast({ message: "请求失败", duration: 2000 });
        httpRequest.destroy();
        return;
      }

      if (data.responseCode === 200 && data.result) {
        try {
          const jsonStr: string = data.result as string;
          const res: Record<string, Object> = JSON.parse(jsonStr) as Record<string, Object>;
          const dataArr: Array<Record<string, Object>> = res['data'] as Array<Record<string, Object>>;

          if (dataArr && dataArr.length > 0) {
            const item = dataArr[0];
            this.temp = item['tmp'] ? String(item['tmp']) : "--";
            this.humi = item['him'] ? String(item['him']) : "--";
            this.gas = item['gan'] ? String(item['gan']) : "--";
            promptAction.showToast({ message: "刷新成功", duration: 2000 });
          }
        } catch (e) {
          promptAction.showToast({ message: "解析失败", duration: 2000 });
        }
      }
      httpRequest.destroy();
    });
  }
}