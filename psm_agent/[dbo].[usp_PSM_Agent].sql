ALTER DATABASE PS_GameLog SET TRUSTWORTHY ON
GO

sp_configure 'clr enabled', 1
GO

reconfigure
GO

USE [PS_GameLog]
GO

CREATE ASSEMBLY [PSM_Agent]
AUTHORIZATION dbo
FROM 'C:\SHAIYA_SERVER\SERVER\PSM_Client\PSM_Agent.dll'
WITH PERMISSION_SET = EXTERNAL_ACCESS

USE [PS_GameLog]
GO

CREATE PROCEDURE [dbo].[usp_PSM_Agent]
@argv [nvarchar](256)
AS EXTERNAL NAME [PSM_Agent].[Console].[Command]
