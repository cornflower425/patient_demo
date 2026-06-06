#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
慢性病患者随访管理系统
Chronic Disease Patient Follow-up Management System
"""

import json
from datetime import datetime
from typing import List, Dict, Optional
import csv
from collections import defaultdict
import os

class FollowUpRecord:
    """随访记录类"""
    
    def __init__(self, follow_up_id: str, patient_id: str, disease: str, 
                 visit_date: str, blood_sugar: float, blood_pressure: str,
                 medication: str, risk_status: str):
        self.follow_up_id = follow_up_id
        self.patient_id = patient_id
        self.disease = disease
        self.visit_date = visit_date
        self.blood_sugar = blood_sugar
        self.blood_pressure = blood_pressure
        self.medication = medication
        self.risk_status = risk_status
    
    def to_dict(self) -> Dict:
        """转换为字典"""
        return {
            'followUpID': self.follow_up_id,
            'patientID': self.patient_id,
            'disease': self.disease,
            'visitDate': self.visit_date,
            'bloodSugar': self.blood_sugar,
            'bloodPressure': self.blood_pressure,
            'medication': self.medication,
            'riskStatus': self.risk_status
        }
    
    def __str__(self) -> str:
        """字符串表示"""
        return (f"随访ID: {self.follow_up_id}\n"
                f"患者ID: {self.patient_id}\n"
                f"疾病: {self.disease}\n"
                f"就诊日期: {self.visit_date}\n"
                f"血糖值: {self.blood_sugar}\n"
                f"血压: {self.blood_pressure}\n"
                f"用药: {self.medication}\n"
                f"风险等级: {self.risk_status}\n"
                f"-" * 40)


class PatientFollowupSystem:
    """患者随访管理系统"""
    
    def __init__(self, csv_file: str = "patient_records.csv"):
        self.records: List[FollowUpRecord] = []
        self.csv_file = csv_file
        self._load_or_initialize_data()
    
    def _load_or_initialize_data(self):
        """从CSV加载数据，如果不存在则初始化示例数据"""
        if os.path.exists(self.csv_file):
            print(f"✓ 从 {self.csv_file} 加载数据...")
            self._load_from_csv(self.csv_file)
        else:
            print("✓ 初始化示例数据...")
            self._initialize_data()
            self._save_to_csv()
    
    def _initialize_data(self):
        """初始化示例数据"""
        data = [
            ("F001", "P001", "高血压", "2026-04-10", 6.2, "148/96", "硝苯地平", "高危"),
            ("F002", "P002", "糖尿病", "2026-04-11", 8.9, "132/84", "二甲双胍", "波动"),
            ("F003", "P003", "冠心病", "2026-04-12", 5.8, "125/80", "阿司匹林", "稳定"),
            ("F004", "P004", "高血脂", "2026-04-13", 5.5, "118/76", "辛伐他汀", "稳定"),
            ("F005", "P005", "高血压+糖尿病", "2026-04-15", 9.6, "152/98", "二甲双胍+氨氯地平", "高危"),
            ("F006", "P006", "脑卒中后遗症", "2026-04-16", 6.1, "138/86", "氯吡格雷", "波动"),
            ("F007", "P007", "糖尿病", "2026-04-17", 7.2, "126/78", "格列美脲", "稳定"),
            ("F008", "P008", "高血压", "2026-04-18", 5.9, "145/92", "厄贝沙坦", "波动"),
            ("F009", "P009", "慢阻肺", "2026-04-20", 6.3, "130/82", "布地奈德", "稳定"),
            ("F010", "P010", "糖尿病", "2026-04-21", 10.5, "136/88", "胰岛素", "高危"),
            ("F011", "P011", "高血压", "2026-04-22", 5.7, "122/75", "美托洛尔", "稳定"),
            ("F012", "P012", "冠心病", "2026-04-23", 6.0, "140/90", "替格瑞洛", "波动"),
            ("F013", "P013", "糖尿病", "2026-04-24", 7.8, "128/80", "利拉鲁肽", "波动"),
            ("F014", "P014", "高血脂", "2026-04-25", 5.4, "119/77", "瑞舒伐他汀", "稳定"),
            ("F015", "P015", "高血压", "2026-04-26", 6.4, "150/95", "贝那普利", "高危"),
        ]
        
        for follow_up_id, patient_id, disease, visit_date, blood_sugar, blood_pressure, medication, risk_status in data:
            record = FollowUpRecord(follow_up_id, patient_id, disease, visit_date, 
                                   blood_sugar, blood_pressure, medication, risk_status)
            self.records.append(record)
    
    def display_menu(self):
        """显示菜单"""
        print("\n" + "=" * 50)
        print("    慢性病患者随访管理系统")
        print("=" * 50)
        print("1. 显示所有随访记录")
        print("2. 按患者ID查询")
        print("3. 按疾病类型查询")
        print("4. 按风险等级查询")
        print("5. 统计分析")
        print("6. 添加新记录")
        print("7. 删除记录")
        print("8. 更新记录")
        print("9. 退出系统")
        print("=" * 50)
        print(f"当前数据文件: {self.csv_file} (共 {len(self.records)} 条记录)")
        print("=" * 50)
        print("请选择操作 (1-9): ", end="")
    
    def display_all_records(self):
        """显示所有记录"""
        print("\n" + "=" * 100)
        print("所有随访记录")
        print("=" * 100)
        print(f"{'序号':<6} {'患者ID':<10} {'疾病':<15} {'就诊日期':<12} {'血糖值':<8} {'血压':<12} {'用药':<25} {'风险':<8}")
        print("-" * 100)
        
        for i, record in enumerate(self.records, 1):
            print(f"{i:<6} {record.patient_id:<10} {record.disease:<15} {record.visit_date:<12} "
                  f"{record.blood_sugar:<8.1f} {record.blood_pressure:<12} {record.medication:<25} {record.risk_status:<8}")
        
        print("-" * 100)
        print(f"共计 {len(self.records)} 条记录\n")
    
    def search_by_patient_id(self):
        """按患者ID查询"""
        patient_id = input("\n请输入患者ID: ").strip()
        results = [r for r in self.records if r.patient_id == patient_id]
        
        if results:
            print(f"\n========== 患者ID为 {patient_id} 的记录 ==========")
            for record in results:
                print(record)
            print(f"找到 {len(results)} 条记录")
        else:
            print(f"未找到患者ID为 {patient_id} 的记录！")
    
    def search_by_disease(self):
        """按疾病类型查询"""
        disease = input("\n请输入疾病名称: ").strip()
        results = [r for r in self.records if disease in r.disease]
        
        if results:
            print(f"\n========== 疾病为 {disease} 的记录 ==========")
            for record in results:
                print(record)
            print(f"找到 {len(results)} 条记录")
        else:
            print(f"未找到疾病为 {disease} 的患者记录！")
    
    def search_by_risk_status(self):
        """按风险等级查询"""
        risk_status = input("\n请输入风险等级 (高危/波动/稳定): ").strip()
        results = [r for r in self.records if r.risk_status == risk_status]
        
        if results:
            print(f"\n========== 风险等级为 {risk_status} 的记录 ==========")
            for record in results:
                print(record)
            print(f"找到 {len(results)} 条记录")
        else:
            print(f"未找到风险等级为 {risk_status} 的患者记录！")
    
    def statistics_analysis(self):
        """统计分析"""
        if not self.records:
            print("暂无记录，无法进行统计！")
            return
        
        print("\n" + "=" * 50)
        print("统计分析")
        print("=" * 50)
        
        # 风险等级分布
        risk_counts = defaultdict(int)
        for record in self.records:
            risk_counts[record.risk_status] += 1
        
        print(f"\n总患者数: {len(self.records)}")
        print("\n风险等级分布:")
        for risk_level in ["高危", "波动", "稳定"]:
            count = risk_counts.get(risk_level, 0)
            percentage = (count / len(self.records)) * 100
            print(f"  - {risk_level}: {count} 人 ({percentage:.1f}%)")
        
        # 血糖值统计
        blood_sugars = [r.blood_sugar for r in self.records]
        print("\n血糖值统计:")
        print(f"  - 平均值: {sum(blood_sugars) / len(blood_sugars):.2f}")
        print(f"  - 最高值: {max(blood_sugars):.2f}")
        print(f"  - 最低值: {min(blood_sugars):.2f}")
        
        # 疾病分布
        disease_counts = defaultdict(int)
        for record in self.records:
            # 处理多个疾病的情况
            diseases = record.disease.split("+")
            for disease in diseases:
                disease_counts[disease.strip()] += 1
        
        print("\n疾病分布:")
        for disease, count in sorted(disease_counts.items(), key=lambda x: x[1], reverse=True):
            print(f"  - {disease}: {count} 人")
        
        # 用药统计
        print("\n常见用药:")
        medication_counts = defaultdict(int)
        for record in self.records:
            medications = record.medication.split("+")
            for medication in medications:
                medication_counts[medication.strip()] += 1
        
        for medication, count in sorted(medication_counts.items(), key=lambda x: x[1], reverse=True)[:5]:
            print(f"  - {medication}: {count} 次")
    
    def add_new_record(self):
        """添加新记录"""
        print("\n" + "=" * 40)
        print("添加新记录")
        print("=" * 40)
        
        try:
            follow_up_id = input("请输入随访ID: ").strip()
            patient_id = input("请输入患者ID: ").strip()
            disease = input("请输入疾病: ").strip()
            visit_date = input("请输入就诊日期 (YYYY-MM-DD): ").strip()
            blood_sugar = float(input("请输入血糖值: "))
            blood_pressure = input("请输入血压 (如 120/80): ").strip()
            medication = input("请输入用药: ").strip()
            risk_status = input("请输入风险等级 (高危/波动/稳定): ").strip()
            
            record = FollowUpRecord(follow_up_id, patient_id, disease, visit_date,
                                   blood_sugar, blood_pressure, medication, risk_status)
            self.records.append(record)
            
            # 立即导出到CSV
            self._save_to_csv()
            print(f"✓ 记录已成功添加并自动保存到 {self.csv_file}！")
        except ValueError:
            print("✗ 输入错误，请检查血糖值是否为数字！")
        except Exception as e:
            print(f"✗ 保存失败: {e}")
    
    def delete_record(self):
        """删除记录"""
        follow_up_id = input("\n请输入要删除的随访ID: ").strip()
        
        for i, record in enumerate(self.records):
            if record.follow_up_id == follow_up_id:
                deleted_record = self.records.pop(i)
                
                # 立即导出到CSV
                try:
                    self._save_to_csv()
                    print("✓ 记录已成功删除并自动保存到文件！")
                    print(f"  删除的记录: {deleted_record.follow_up_id} - {deleted_record.patient_id}")
                except Exception as e:
                    print(f"✗ 保存失败: {e}")
                return
        
        print("✗ 未找到该随访ID的记录！")
    
    def update_record(self):
        """更新记录"""
        follow_up_id = input("\n请输入要更新的随访ID: ").strip()
        
        for record in self.records:
            if record.follow_up_id == follow_up_id:
                try:
                    print(f"\n当前信息:")
                    print(f"  血糖值: {record.blood_sugar}")
                    print(f"  血压: {record.blood_pressure}")
                    print(f"  用药: {record.medication}")
                    print(f"  风险等级: {record.risk_status}")
                    print()
                    
                    record.blood_sugar = float(input("请输入新的血糖值: "))
                    record.blood_pressure = input("请输入新的血压: ").strip()
                    record.medication = input("请输入新的用药: ").strip()
                    record.risk_status = input("请输入新的风险等级 (高危/波动/稳定): ").strip()
                    
                    # 立即导出到CSV
                    self._save_to_csv()
                    print(f"✓ 记录已成功更新并自动保存到 {self.csv_file}！")
                    return
                except ValueError:
                    print("✗ 输入错误，请检查血糖值是否为数字！")
                    return
                except Exception as e:
                    print(f"✗ 保存失败: {e}")
                    return
        
        print("✗ 未找到该随访ID的记录！")
    
    def _save_to_csv(self):
        """保存到CSV文件"""
        try:
            with open(self.csv_file, 'w', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                # 写入表头
                writer.writerow(['FollowUpID', 'PatientID', 'Disease', 'VisitDate', 
                               'BloodSugar', 'BloodPressure', 'Medication', 'RiskStatus'])
                # 写入数据
                for record in self.records:
                    writer.writerow([record.follow_up_id, record.patient_id, record.disease,
                                   record.visit_date, record.blood_sugar, record.blood_pressure,
                                   record.medication, record.risk_status])
        except Exception as e:
            print(f"✗ 保存CSV文件失败: {e}")
            raise
    
    def _load_from_csv(self, filename: str):
        """从CSV文件加载"""
        try:
            with open(filename, 'r', encoding='utf-8') as f:
                reader = csv.reader(f)
                header = next(reader)  # 跳过表头
                
                for row in reader:
                    if len(row) >= 8:
                        try:
                            record = FollowUpRecord(
                                row[0], row[1], row[2], row[3],
                                float(row[4]), row[5], row[6], row[7]
                            )
                            self.records.append(record)
                        except ValueError:
                            continue
        except Exception as e:
            print(f"✗ 加载CSV文件失败: {e}")
            print("✓ 使用默认示例数据...")
            self._initialize_data()
    
    def run(self):
        """运行系统"""
        print("欢迎使用慢性病患者随访管理系统！")
        print(f"数据文件: {self.csv_file}")
        
        while True:
            self.display_menu()
            
            try:
                choice = input().strip()
                
                if choice == '1':
                    self.display_all_records()
                elif choice == '2':
                    self.search_by_patient_id()
                elif choice == '3':
                    self.search_by_disease()
                elif choice == '4':
                    self.search_by_risk_status()
                elif choice == '5':
                    self.statistics_analysis()
                elif choice == '6':
                    self.add_new_record()
                elif choice == '7':
                    self.delete_record()
                elif choice == '8':
                    self.update_record()
                elif choice == '9':
                    print(f"\n✓ 所有数据已自动保存到 {self.csv_file}")
                    print("感谢使用本系统，再见！")
                    break
                else:
                    print("✗ 无效的选择，请重新输入！")
            
            except KeyboardInterrupt:
                print("\n\n✓ 所有数据已自动保存")
                print("系统已中断，再见！")
                break
            except Exception as e:
                print(f"✗ 发生错误: {e}")


def main():
    """主函数"""
    # 可以自定义CSV文件名
    csv_filename = "patient_records.csv"
    system = PatientFollowupSystem(csv_filename)
    system.run()


if __name__ == "__main__":
    main()
